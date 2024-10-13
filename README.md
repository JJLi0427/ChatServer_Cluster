# ChatServer_Learn

### Use it Before

Install `muduo` `MySQL` `Nginx` `Redis`  

### How to install and config:

**nginx.conf**:
```
stream {
    upstream ChatServer {
        server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
        server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
    }
    server {
        proxy_connect_timeout 1s;
        listen 6666;
        proxy_pass ChatServer;
        tcp_nodelay on;
    }
}
```

``` shell
git clone
cd ChatServer_Learn
chmod a+x ./install.sh
./install.sh
```

### How to use:
**Init ChatServer**
```
cd ./bin
# eg.start 2 server
./ChatServer 127.0.0.1 6000
./ChatServer 127.0.0.1 6002
# start a client
./ChatClient 127.0.0.1 6666
```
