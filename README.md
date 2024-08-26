# ChatServer_Learn

### Use it Before

Install `muduo` `MySQL` `Nginx` `Redis`  

### How to use it:

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
./install.sh
```