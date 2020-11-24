# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure('2') do |config|
  config.vm.box = 'bento/ubuntu-20.04'
  config.vm.synced_folder '.', '/home/vagrant/src'
  config.vm.provision :shell,
                      privileged: true,
                      inline: <<SCRIPT
#!/bin/bash -e
apt-get update
apt-get install -y build-essential \
  freeglut3-dev \
  libjpeg-dev libpng-dev \
  lxqt-core sddm # Might need to do a `vagrant reload` afterwards to get the UI to load
SCRIPT
end
