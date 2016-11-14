# WebService

   Fastcgi + lighttpd , here just achieve the basic  websevice frame in oder to tell you
how to program using c ， you need config the fastcgi in the lighttpd.congf ,for example:

fastcgi.server += (
  // your root  url 
   "/devices" => (
   
    "test.fastcgi.handler" => (
    
      "socket" => "/tmp/WebService.fastcgi.socket", // your webservice socket in your code
      
      "allow-x-sendfile"  => "enable",
      
      "check-local" => "disable",
    )
    
  )
  
)

