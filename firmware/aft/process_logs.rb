#!/usr/bin/env ruby

# args:
#     [0] = directory of logs or a specific file name ending in either ".ADL" or ".ADH"

require 'fileutils'
require 'date'
require 'openssl'

$using_dir = false
$using_file = false
$dir_name = ''
$file_name = ''
$decryptedFile = ''
$dataCount = 0
$header_found = Hash.new # maps app_log_id => whether the ADH file could be found for that ADL

# check command line arguments
if ARGV.length > 0
   if ARGV[0][-3..-2] == "AD"
      $using_file = true
      $dir_name = ARGV[0][0..-13] # get of the file name
      $file_name = ARGV[0].split("/").last
   else
      $using_dir = true
      $dir_name = ARGV[0]
   end
else
   puts "No folder/file argument given."
   puts "Usage: command <location of log folder/file>"
   abort()
end

$list_of_files = Array.new

###
# Loads all files from a given directory, or if user just wants 1 file,
# (using_file = true), it will just load the ADH and ADL for that file_name
###
def get_files()
   Dir.foreach($dir_name) do |file|
      if (file[-3..-1] != "ADL" and file[-3..-1] != "ADH") or
         ($using_file and file[0..-5] != $file_name[0..-5])
         next
      else
         $list_of_files.push(file)
      end

      if file[-3..-1] == "ADH"
         $header_found[file[0..-5]] = true
      elsif $header_found[file[0..-5]] != true
         $header_found[file[0..-5]] = false
      end

   end

   return $list_of_files
end

###
# Sorts an Array of ADH/ADL files into chronological order
###
def sort_files(array_of_numbers, index_to_sort = 0, lower_bound = 0,
               upper_bound = array_of_numbers.length-1)
   return if index_to_sort > 7

   for i in lower_bound..upper_bound
      j = i
      while j > lower_bound and array_of_numbers[j-1][index_to_sort] >
                                 array_of_numbers[j][index_to_sort]
         # swap files
         temp = array_of_numbers[j]
         array_of_numbers[j] = array_of_numbers[j-1]
         array_of_numbers[j-1] = temp

         j -= 1
      end
   end

   new_low = lower_bound
   curr_num = array_of_numbers[lower_bound][index_to_sort]
   num_repeated = 0
   for i in lower_bound+1..upper_bound
      if array_of_numbers[i][index_to_sort] == curr_num
         num_repeated += 1
         if i == upper_bound
            sort_files(array_of_numbers, index_to_sort+1, new_low, i)
         end
      elsif num_repeated > 0
         sort_files(array_of_numbers, index_to_sort+1, new_low, i-1)
         num_repeated = 0
         new_low = i
         curr_num = array_of_numbers[i][index_to_sort]
      else
         curr_num = array_of_numbers[i][index_to_sort]
         new_low = i
      end
   end
end

###
# gets a specified number of bytes from a file starting at start_byte
###
def decryptFile(file)
   	$decryptedFile = $cipher.update(file) + $cipher.final()
end

###
# gets a specified number of bytes from a file starting at start_byte
###
def getBytes(start_byte, num_bytes_to_read)
   return $decryptedFile[start_byte, num_bytes_to_read]
end

###
# get human-readable datetime
###
def getDateTimeText(dt)
   begin
      y = dt[0] + 2000
      mlz = dt[4] < 10 ? "0" : ""
      slz = dt[5] < 10 ? "0" : ""
      return "%d-%d-%d %d:%s%d:%s%d" % [y,dt[1],dt[2],dt[3],mlz,dt[4],slz,dt[5]]
   rescue NoMethodError # nil
      return -1
   end
end

$cipher = OpenSSL::Cipher::AES.new(128, :ECB)
$cipher.decrypt
$cipher.padding = 0
$cipher.key = "dkd874hcEI78Bfe\0"

Log_Entry = Struct.new(:file, :app_log_id, :values)
$list_of_logs = Array.new # holds all of the log entries

###
# parses a given file for log information
###
def get_ADL_information(file)
   f = IO.binread($dir_name+file)
   #decryptFile(f)
   puts "Processing data file..."

   f_index = 0
   data = ''
   first_entry = true
   time_tick = 1
   # -----------------------------------------------------
   # ADL data is in the format of:
   # app's log id , number of values , timestamp , values
   #      1B      ,         1B       ,     6B    , each value is 2B with any # of values
   # -----------------------------------------------------
   catch (:done) do
      while true
         # reset variables
         app_log_id = ''
         num_values = ''
         values = ''
         log_time = ''

         # get app's log id
         begin
            while true
               data = f[f_index, 1] #getBytes(f_index, 1)
               f_index += 1
               if data.unpack('C')[0] != 0
                  app_log_id = data.ord
                  break
               end
            end
         rescue
            throw :done
         end

         # get the number of values
         data = f[f_index, 1] #getBytes(f_index, 1)
         f_index += 1
         num_values = data.ord

         # get log_time
         data = f[f_index, 6] #getBytes(f_index, 6)
         f_index += 6
         if first_entry
            log_time = getDateTimeText(data.unpack('C6'))
            break if log_time == -1
            values += log_time.to_s + ","
            first_entry = false
         else
            values += (time_tick * 0.0500000).round(3).to_s + "," # BSP samples at 20Hz from both sensors at RTOS level
            time_tick = time_tick + 1
         end
         # get values
         for i in (1..num_values)
            data = f[f_index, 4] #getBytes(f_index, 4)
            # puts data.inspect
            $dataCount += 1
            if ($dataCount % 100000) == 0 
            	puts "Point number..."
            	puts $dataCount
            end
            f_index += 4
            if data.unpack('C') != 0
               values += data.unpack('F')[0].to_s + ","  # F for unpacking float. s for short int.:
            end

            if i == num_values
               values[-1] = "\n"
            end
         end

         # put all values with the same app log id in the same Log Entry
         # make a new Entry for new app log ids
         id_found = false
         for log in $list_of_logs
            if log[:file] == file[0..-5] and log[:app_log_id] == app_log_id
               log[:values] += values
               id_found = true
            end
         end

         if !id_found
            $list_of_logs.push(Log_Entry.new(file[0..-5], app_log_id, values))
         end

      end
   end
end

Header = Struct.new(:file, :app_name, :app_log_id, :value_headers_size, :value_headers)
$list_of_headers = Array.new # holds all of the headers

###
# parses a given file for header information
###
def get_ADH_information(file)
   f = IO.binread($dir_name+file)
   decryptFile(f)

   f_index = 0
   data = ''

   # ----------------------------------------------------
   # ADH data is in the format of:
   # ! , app's name , 00 , value headers
   # ----------------------------------------------------
   # + ! is used to signify the beginning of a new header entry
   # + the app's name has a comma after it to signify the endif
   # + there are 2 zeros because the app's name doesn't log properly without them
   # + There can be any number of value headers. We can tell when they are done by
   #   either 0's or the ! marking the next header entry
   # ----------------------------------------------------
   catch(:done) do
      loop do
         # reset variables
         app_name = ''
         app_log_id = ''
         value_headers_size = ''
         value_headers = ''

         # get app name
         while true
            data = f[f_index, 1] #etBytes(f_index, 1)
            f_index += 1

            if data.nil?
               throw(:done)
            elsif data == ','
               break
            elsif data.unpack('C')[0] == 0
               next
            elsif data != '!'
               # we don't want spaces in the name, so change them to underscores
               if data == ' '
                  app_name += '_'
               else
                  app_name += data
               end
            end
         end

         # get app log id
         while true
            data = f[f_index, 1] #getBytes(f_index, 1)
            f_index += 1

            if data.unpack('C')[0] != '0'
               app_log_id = data.ord
               break
            end
         end

         # get value_headers_size
         data = f[f_index, 1] #getBytes(f_index, 1)
         f_index += 1
         if data == ''
            break
         end

         value_headers_size = data.ord

         # get value_headers
         for i in 1..value_headers_size
            data = f[f_index, 1] #getBytes(f_index, 1)
            f_index += 1

            if data.ord > 0 && data.ord < 128 # ASCII values range from 0-127, but 0 is the end of a string
               value_headers += data
            end
         end

         # make sure no duplicate headers are logged
         header_is_duplicate = false
         for header in $list_of_headers
            if header[:file] == file[0..-5] and header[:app_name] == app_name
               header_is_duplicate = true
               break
            end
         end

         next if header_is_duplicate

         # Each log entry has a time value, but the user isn't required to add this
         # to the value_headers, so we need to do that here.
         value_headers = "DateTime," + value_headers
         value_headers_size += 5
         $list_of_headers.push(Header.new(file[0..-5], app_name, app_log_id, value_headers_size, value_headers))

         break if data == ''
      end
   end
end

###
# If a header file isn't found with the ADL file, see if that ADL
# has been logged in the past when it was with its header file
###
def check_old_ADHs(file)
   # see if header info is already logged in /log_headers/
   Dir.foreach("log_headers/") do |h_file|
      if h_file != file[0..-4]+"ADH"
         next
      else
         $header_found[file[0..-5]] = true

         tmp_dir = $dir_name
         $dir_name = "log_headers/"

         get_ADH_information(h_file)

         $dir_name = tmp_dir
      end
   end
end

###
# Creates file structure for logs/headers
###
def make_directories()
   # get current time for folder name
   current_time = Time.now.strftime("%m-%d-%y_%H%M")

   # make directories
   FileUtils.mkdir_p("log_files") unless File.directory?("log_files")
   FileUtils.mkdir_p("log_headers") unless File.directory?("log_header")

   log_dir = "logs_" + current_time
   FileUtils.cd("log_files")
   if !File.exists?(log_dir)
      FileUtils.mkdir_p(log_dir)
   else
      file_offset = 1

      log_dir = log_dir + "_"
      while File.exists?(log_dir+file_offset.to_s)
         file_offset += 1
      end
      log_dir += file_offset.to_s

      FileUtils.mkdir_p(log_dir)
   end

   FileUtils.cd(log_dir)
end

###
# Writes logs to csv's
###
def write_logs()
   # write csv's for each app using ADLs that had their respective ADHs
   for header in $list_of_headers
      for log in $list_of_logs
         file_exists = false

         if header != nil and header[:file] != -1
            if header[:file] == log[:file] and
               header[:app_log_id] == log[:app_log_id]

               if File.exists?(header[:app_name]+".csv")
                  file_exists = true
               end

               File.open(header[:app_name]+".csv", 'a') { |file|
                  if !file_exists
                     file.write(header[:value_headers] + "\n")
                  end
                  file.write(log[:values])
               }
            end
         end
      end
   end

   # write any ADLs that didn't have ADHs out to csv's
   $header_found.each { |file, found|
      if "#{found}" == "false"
         for log in $list_of_logs
            if log[:file] == "#{file}"
               if File.exists?(log[:app_log_id].to_s+".csv")
                  file_exists = true
               end

               name = log[:app_log_id].to_s

               file_offset = 1
               while file_exists and File.exists?(name+"_"+file_offset.to_s+".csv")
                  file_offset += 1
               end

               if file_exists
                  name += "_"+file_offset.to_s
               end

               File.open(name+".csv", 'w') { |file|
                  file.write("DateTime\n")
                  file.write(log[:values])
               }
            end
         end
      end
   }
end

###
# Writes .ADLs to /log_headers/ directory
###
def write_headers()
   for file in $list_of_files
      if file[-3..-1] == "ADH"
         FileUtils.cp($dir_name+file, "../../log_headers/"+file)
      end
   end
end

#######################################################################
# START
#######################################################################
$list_of_files = get_files()

if $using_dir
   sort_files($list_of_files)
end

# parse each file
header_file_found = true
for file in $list_of_files
   catch(:done) do
      if file[-3..-1] == "ADL"
         begin
            get_ADL_information(file)
         rescue ArgumentError # we've reached the end of the file
            throw :done # stop parsing file
         end

         if $header_found[file[0..-5]] == true
            get_ADH_information(file[0..-4]+"ADH")
         else
            check_old_ADHs(file)
         end

      end
   end
end

make_directories()

write_logs()

write_headers()
