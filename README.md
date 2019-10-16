# vht - Virtual Host Tool

A quick `apache2` virtual host creation tool for development machines.

## Instructions

To install

```
git clone https://github.com/isdampe/vht
cd vht
make && sudo make install
```

To use

```
cd /path/to/somehost.site
sudo vhost somehost.site
```

or

```
sudo vhost somehost.site /path/to/somehost.site
```
