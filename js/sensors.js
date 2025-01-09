const SUCCESS = 200;
// Tag id : A2D 
 function readA2D(){
    const startDateTime = Date.now();

    setTimeout(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function(){
            if(this.readyState === 4 && this.status === 200){
                const respObject = JSON.parse(this.response);
                document.getElementById("analog_data").innerText = respObject.value;
            }
        };
        xhttp.open("GET","/a2d", true);
        xhttp.send();
    }, 2000);

}

 function uploadFile() {  
    var formData = new FormData();  
    var fileField = document.getElementById("fileInput").files[0];  
    var fileType = document.getElementById("lstFileType").value;

    formData.append("file", fileField);  
  
    var xhr = new XMLHttpRequest();  
    xhr.open("POST", "/upload?type="+fileType, true);  
  
    xhr.onload = function() {  
      if (xhr.status === 200) {  
        console.log("File uploaded successfully");  
        document.getElementById("dismiss_dialog").click();
      } else {  
        console.error("Upload failed");  
      } 
    };  
  
    xhr.send(formData);  
  }  
  

 function readConnectionStatus() {
    const startDateTime = Date.now();
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200){
            const respObject = JSON.parse(this.response);
            document.getElementById("wifi_addr").innerText = "آدرس IP : " + " " + respObject.ip_addr;
        }
    }
    xhttp.open("GET","/con_info", true);
    xhttp.send();
}


function readHardwareInfo() {
    const startDateTime = Date.now();
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200){
            const respObject = JSON.parse(this.response);
            document.getElementById("hw_info").innerText = "نسخه سخت افزار : " + " " + respObject.hw_ver;
        }
    }
    xhttp.open("GET","/hw_info", true);
    xhttp.send();
}

function readSDInfo() {
    const startDateTime = Date.now();
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200){
            const respObject = JSON.parse(this.response);
            const sizeInMB = (respObject.card_size / 1024 / 1024 / 1024 )
            document.getElementById("sd_info").innerText = "سایز استفاده شده : " + " " + sizeInMB + " مگابایت ";
        }
    }
    xhttp.open("GET","/sd_info", true);
    xhttp.send();
}