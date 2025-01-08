
// Tag id : A2D 
async function readA2D(){
    const startDateTime = Date.now();

    setTimeout(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function(){
            if(this.readyState === 4 && this.status === 200){
                document.getElementById("analog_data").innerText = this.response.value;
            }
        };
        xhttp.open("GET","/a2d", true);
        xhttp.send();
    }, 2000);

}

async function uploadFile() {  
    var formData = new FormData();  
    var fileField = document.getElementById("fileInput").files[0];  
    var fileType = document.getElementById("lstFileType").value;

    formData.append("file", fileField);  
  
    var xhr = new XMLHttpRequest();  
    xhr.open("POST", "/upload?type="+fileType, true);  
  
    xhr.onload = function() {  
      if (xhr.status === 200) {  
        console.log("File uploaded successfully");  
      } else {  
        console.error("Upload failed");  
      }  
    };  
  
    xhr.send(formData);  
  }  
  

async function readConnectionStatus() {
    const startDateTime = Date.now();
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200){

        }
    }
    xhttp.open("GET","/cn_info", true);
    xhttp.send();
}