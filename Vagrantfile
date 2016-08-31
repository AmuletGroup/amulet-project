
# Copyright 2016 by the Trustees of Dartmouth College 
# and Clemson University, and distributed under the terms
# of the "Dartmouth College Non-Exclusive Research Use Source 
# Code License Agreement" (for NON-COMMERCIAL research purposes only),
# as detailed in a file named LICENSE.pdf within this repository.

# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
	# All Vagrant configuration is done here. The most common configuration
	# options are documented and commented below. For a complete reference,
	# please see the online documentation at vagrantup.com.

	# Adding project amulet repository to proper directory
	config.vm.synced_folder ".", "/home/vagrant/Repos/project-amulet", owner: "vagrant"
	config.vm.synced_folder ".", "/vagrant", disabled: true
	config.vm.hostname = "vagrant"
	# Every Vagrant virtual environment requires a box to build off of.
	config.vm.box = "ubuntu/trusty64"
	#config.vm.hostname = "Amulet-Dev-Box"
	config.vm.provision "shell", path: "./bootstrap.sh"

	# Share an additional folder to the guest VM. The first argument is
	# the path on the host to the actual folder. The second argument is
	# the path on the guest to mount the folder. And the optional third
	# argument is a set of non-required options.

	# Enable USB access
	usb_devs = [
		# Required for programming new fraunchpad
		['0x2047', '0x0013', 'MSP4305969 Launchpad programmer'],
		['0x2047', '0x0203', 'MSP4305969 Launchpad FW updater'],
		# Other USB ids from TI's udev rules.
		['0x2047', '0x0010', 'MSP430UIF'],
		['0x2047', '0x0014', 'MSP430UIF'],
		['0x2047', '0x0204', 'MSP430UIF'],
		# For older fraunchpads and launchpads
		['0x0451', '0xf432', 'eZ430'],
		# Misc devices
		['0x15ba', '0x0031', 'Olimex JTAG tiny'],
	]

	# Provider-specific configuration so you can fine-tune various
	# backing providers for Vagrant. These expose provider-specific options.
	# Example for VirtualBox:
	config.vm.provider "virtualbox" do |vb|

		vb.name = "Amulet Box"
		vb.customize ["modifyvm", :id, "--memory", "1024"]
		vb.customize ['modifyvm', :id, '--usb', 'on']

		# Don't boot with headless mode
		vb.gui = true

		usb_devs.each do |dev|
			vb.customize ['usbfilter', 'add', '0', '--target', :id, '--vendorid', dev[0], '--productid', dev[1], '--name', dev[2]]
    	end
	end
end
