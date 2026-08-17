### What is a WebServer 

At it most basic level, a webSever is just a continously running program that listen for incoming network connections. Its primary fct is to store, process, and deliver web pages to clients. 


When a client (usually a web brower or with curl) request a website (html js and css ), it initiates communication by requesting a specific resources from the server. 

The server receives that request, search its hard drive for the resquested resource and send it back (the html file or CGI script for example). If it can't find it, or ain't allowed to serve it, it replies with a specific error msg. 

## What is an HTTP Request 

HTTP (Hypertext Tranfert Protocol) is the actual language that the client use to communicate with the server. It is an application protocol developed to support Hypertext fuctionality and is the foundation of data communication for the World Wide Web. 

When the brower connect to the server, it sends a block of text formatted according to the HTTP rules. This block of text is the *HTTP Request* 

While the HTTP's first mission is to serve content, it also enables clients to send data to the server, which is used for submitting web forms or uploading files. 


This is what a standard HTTP request looks like

```bash
GET /tours/vacation.html HTTP/1.1
Host: localhost:8002
User-Agent: Mozilla/5.0
Accept: text/html
```

The server needs to get into this raw text and undersand : 
1. The Method (In our case its GET): what does the client wants ? Retrieve a file or send data (POST) 
2. *The Target/URL* (/tours/vactions.html): What exact resource the client is looking for ?
3. *The Header* (host: ...): Extra metadata about the request 

## How the config file works 

So this the most important part of the webServe, the *RuleBook*. If you can understand this, then building it just becoming a nightmare in coding it. 

# 1. The General Rules 


When the server first receives a connect on port 8080, it looks at the global settings inside the server {...} block. These apply to everything unless a specific location says otherwise, but let forget that for now. and just focus on the global thing. 
```listen 8002 & host 127.0.0.1``` 
for the server it's : I only answer to the call if it comes to this exact IP and port 
```client_max_body_size 3000000``` : If someone tires to upload a file larger than 3MB, I will reject it.  
``` error page 404 error_pages/404.html``` : If I can't find the file, send this specific page as an error msg. 

# 2. Our lovely couple root and index
Those two lovers dictate how an incoming URL is translated into a physical file in the hard drive
1. ```root```: This is the base directory on the computer, where the resquested file should be located. It is the starting point. 
2. ```index```: This is the default file to serve when the requested resource is a directory than a specific file. ```127.0.0.1:8080/```

What is better than real examples to understand, right? 

*Scenario 1: The root location (location/)* 

``` 
root docs/fusion_web/; # Inherited from the global server block
index index.html;      # Inherited from the global server block

location / {
    allow_methods  DELETE POST GET; 
    autoindex off;                   
}
```
Here the client asks for : ``` http://localhost:8080/```. It is asking for the root directory ```\```. Because It asked for a folder not a file, the server checks the ```index``` rule. It appends ``` index.html ``` to the ```root```. 

