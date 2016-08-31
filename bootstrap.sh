#!/usr/bin/env bash

if cat ~/.ssh/config |grep "Host buffet.cs.clemson.edu"
	then
	echo "already have buffet"
else
	sudo printf '%s\n    %s\n' 'Host buffet.cs.clemson.edu' 'StrictHostKeyChecking no' >> ~/.ssh/config
fi
#ssh -o StrictHostKeyChecking=no buffet.cs.clemson.edu uptime

if ls /etc/apt/sources.list.d | grep webupd8
	then
	echo "Already have sources"
else
	echo "Need to add souces----adding now"
	sudo add-apt-repository -y ppa:nilarimogard/webupd8 2>&1
	sudo add-apt-repository -y ppa:webupd8team/java 2>&1
	sudo add-apt-repository -y ppa:webupd8team/sublime-text-3 2>&1
	sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu $(lsb_release -sc) main universe restricted multiverse"
	sudo dpkg --add-architecture i386
fi

sudo apt-get update > /dev/null 2>&1
echo "Installing ubuntu desktop"
sudo apt-get install ubuntu-desktop -y > /dev/null 2>&1
echo "Done installing ubuntu desktop"
sudo apt-get install -f > /dev/null 2>&1
echo "Upgrading Packages"
sudo apt-get upgrade -y > /dev/null 2>&1
echo "Installing packagaes"
sudo apt-get update > /dev/null 2>&1

sudo apt-get install -y build-essential > /dev/null 2>&1
sudo apt-get install -y libc6:i386 libncurses5:i386 libstdc++6:i386 > /dev/null 2>&1
sudo apt-get install linux-headers-$(uname -r) > /dev/null 2>&1
sudo apt-get install -y git rake mercurial zsh ssh subversion > /dev/null 2>&1
sudo apt-get install -y libdvdread4 > /dev/null 2>&1
sudo apt-get install -y libusb-dev:i386 > /dev/null 2>&1
sudo apt-get install -y libreadline-dev:i386 > /dev/null 2>&1
sudo apt-get isntall -y libusb-dev > /dev/null 2>&1
sudo apt-get install -y libreadline-dev > /dev/null 2>&1
sudo apt-get install -y gcc-multilib g++-multilib > /dev/null 2>&1
sudo apt-get install -y zsh > /dev/null 2>&1
sudo apt-get install -y libdvdread4 > /dev/null 2>&1
sudo apt-get install -y zip unzip > /dev/null 2>&1
sudo apt-get install -y subversion unity-tweak-tool > /dev/null 2>&1
sudo apt-get install -y libxss1 > /dev/null 2>&1
sudo apt-get install -y gtk2-engines-murrine:i386 gtk2-engines-pixbuf:i386 sni-qt:i386 ubuntu-restricted-extras libc6:i386 libx11-6:i386 libasound2:i386 libatk1.0-0:i386 libcairo2:i386 libcups2:i386 libdbus-glib-1-2:i386 libgconf-2-4:i386 libgdk-pixbuf2.0-0:i386 libgtk-3-0:i386 libice6:i386 libncurses5:i386 libsm6:i386 liborbit2:i386 libudev1:i386 libusb-0.1-4:i386 libstdc++6:i386 libxt6:i386 libxtst6:i386 libgnomeui-0:i386 libusb-1.0-0-dev:i386 libcanberra-gtk-module:i386 libxss1 > /dev/null 2>&1
sudo apt-get install -y gcc-arm-none-eabi > /dev/null 2>&1
sudo apt-get install -y mspdebug:i386 > /dev/null 2>&1
sudo apt-get install -y ruby > /dev/null 2>&1
sudo apt-get install -y terminator sublime-text-installer > /dev/null 2>&1
sudo apt-get install -y linux-image-extra-virtual > /dev/null 2>&1
sudo apt-get install -y virtualbox-guest-dkms > /dev/null 2>&1

sudo echo oracle-java7-installer shared/accepted-oracle-license-v1-1 select true | \
sudo /usr/bin/debconf-set-selections
sudo apt-get install -y oracle-java7-installer 2>&1
echo "Done with packages now adding other tools"

if ls -lah |grep .oh-my-zsh
	then
	echo "Already have oh-my-zsh"
else
	git clone -q git://github.com/robbyrussell/oh-my-zsh.git /home/vagrant/.oh-my-zsh
fi

rm .zshrc > /dev/null 2>&1
wget -q https://raw.githubusercontent.com/kevinafreeman/dot-files/master/zshrc1 -O .zshrc -P /home/vagrant/
rm -rf ~/.oh-my-zsh/themes/kevin.zsh-theme
wget -q https://raw.githubusercontent.com/kevinafreeman/dot-files/master/kevin1.zsh-theme -P /home/vagrant/.oh-my-zsh/themes/ 

mspdebug tilib > msp.text

if grep r.text "MSP430.dll v3.3.1.4"; then
	echo "Already have mspdebug"
else
	printf '%s\n    %s\n' 'Host buffet.cs.clemson.edu' 'StrictHostKeyChecking no' >> ~/.ssh/config
	git clone anonymous@buffet.cs.clemson.edu:jhester/mspdebug-fresh
	cd mspdebug-fresh
	make
	sudo cp mspdebug /usr/bin/mspdebug
	cd ..
	rm -rf mspdebug-fresh
	fi
rm msp.text

if ls /opt |grep ti-mspgcc; then
	rm -rf /opt/ti-mspgcc
fi

TI_MSPGCC_URL=http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/3_04_05_01/exports/msp430-gcc-full-linux-installer-3.4.5.1.run
TI_MSPGCC_DIR=/opt/ti-mspgcc

echo "Downloading TI MSPGCC"
wget -qO installer $TI_MSPGCC_URL
echo "Installing TI MSPGCC"
chmod +x installer
./installer --mode unattended --prefix $TI_MSPGCC_DIR
# Copy headers and ldscripts to the correct location to prevent the need to explicitly include them
cp $TI_MSPGCC_DIR/{include/*.h,msp430-elf/include}
cp $TI_MSPGCC_DIR/{include/*.ld,msp430-elf/lib}
rm -rf installer

echo "export PATH=$TI_MSPGCC_DIR/bin:$PATH" >> /etc/profile
$TI_MSPGCC_DIR/install_scripts/msp430uif_install.sh

sudo rm -rf /usr/lib/libmsp430.so
sudo ln -s /opt/ti-gcc/bin/libmsp430.so /usr/lib/

if grep "LD_LIBRARY_PATH" /home/vagrant/.zshrc; then
	echo "Have LD_LIBRARY_PATH already"
else
	sudo echo ``export LD_LIBRARY_PATH=/opt/ti-mspgcc/bin`` >> /etc/profile
fi

echo "Installing QM"
rm -rf /home/vagrant/qm
wget -q http://sourceforge.net/projects/qpc/files/QM/3.3.0/qm_3.3.0-linux64/download
chmod +x download
./download --mode silent --prefix /home/vagrant/qm
rm download
echo "Done installing QM"
sudo userdel ubuntu > /dev/null 2>&1
sudo chsh -s /bin/zsh vagrant
echo "Done building Vagrant VM"
