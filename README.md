rlogr
=====

stdin to redis logger

	tail -0f /var/log/syslog | rlogr -s syslog
	
install
======

	scons
	scons install # /usr/local/bin/rlogr
	
	
usage
=====
```
Redirect stdout to redis set v0.9.0
	-h redis hostname [127.0.0.1]
	-p redis port [6379]
	-s redis setname (required)
	-t transparently dump output
```