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
apt-get install -y \
  build-essential \
  mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev \
  libsdl2-dev libsdl2-image-dev \
  lxqt-core sddm
SCRIPT
  config.vm.provision :reload # May need to `vagrant plugin install vagrant-reload` first.
end
