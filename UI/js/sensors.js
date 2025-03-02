const SUCCESS = 200;

function uploadFile() {
  const formData = new FormData();
  const fileField = document.getElementById("fileInput").files[0];
  const fileType = document.getElementById("lstFileType").value;

  formData.append("file", fileField);

  const xhr = new XMLHttpRequest();
  xhr.open("POST", "/upload?type=" + fileType, true);

  xhr.onload = function () {
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
     const xhttp = new XMLHttpRequest();
     xhttp.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200){
            const respObject = JSON.parse(this.response);
            document.getElementById("wifi_addr").innerText = "آدرس IP : " + " " + respObject.ip_addr;
        }
    }
    xhttp.open("GET","/con_info", true);
    xhttp.send();
}

function changeserverstatus(){
  var xhttpstatus = new XMLHttpRequest();
    xhttpstatus.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var a = this.responseText;
        if(a=="up"){
          document.getElementById("isupid").style.backgroundColor = "#BLUE";
          document.getElementById('isupid').onclick = function() { sendUpDn(1,0) };
        }
        if(a=="down"){
          document.getElementById("isdownid").style.backgroundColor = "#BLUE";
          document.getElementById('isdownid').onclick = function() { sendUpDn(0,1) };
        }
      }
    };
    xhttpstatus.open("GET", "/check", true);
    xhttpstatus.send();
}

function sendUpDn(mtrUP_flag,mtrDN_flag)  //------0--------------------------
{
  var xhttpMotor = new XMLHttpRequest();
  xhttpMotor.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var b= this.responseText;
      
       if("b==00"){
        
        document.getElementById("isupid").style.backgroundColor = "#BLUE";
        document.getElementById('isupid').onclick = function() { sendUpDn(1,0) };
        document.getElementById("isdownid").style.backgroundColor = "#BLUE";
        document.getElementById('isdownid').onclick = function() { sendUpDn(0,1) };
       }
       else{}
       if (b=="10"){
       document.getElementById("isupid").style.backgroundColor = "red";
       document.getElementById('isupid').onclick = function() { sendUpDn(0,0) };
       }
       else{}
       if (b=="01"){
       document.getElementById("isdownid").style.backgroundColor = "red";
       document.getElementById('isdownid').onclick = function() { sendUpDn(0,0) };
       }
       else{}
    }
  };
  xhttpMotor.open("GET", "MotorSet?state="+mtrUP_flag+mtrDN_flag, true);
  xhttpMotor.send();
}

setInterval(function(){ changeserverstatus(); } , 300); 
setInterval(function(){ getData(); } , 350); 

function getData() {
  var xhttpADC = new XMLHttpRequest();
  xhttpADC.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("adc_val").innerHTML = this.responseText;
    }
  };
  xhttpADC.open("GET", "adcread", true);
  xhttpADC.send();
}

function SetKey(){
  var RemoteNumber;
  var KeyNumber;
  var ProgGroupNumber;
  var xhttpRmtky = new XMLHttpRequest();
  xhttpRmtky.onreadystatechange = function() {
/*    if (this.readyState == 4 && this.status == 200) {
      RemoteNumber=document.getElementById("RemoteNoID").value;
    }
  */};
    //document.getElementById("RemoteNoID").value =+1;
      RemoteNumber=document.getElementById("RemoteNoID").value;
      KeyNumber=document.getElementById("KeyNoID").value;
      ProgGroupNumber=document.getElementById("ProgGroupID").value;
      var rmtky=
                parseInt(RemoteNumber *0x10000)+
                parseInt(KeyNumber*0x100)+
                parseInt(ProgGroupNumber);
//      alert(rmtky);
  xhttpRmtky.open("POST", "/RemoteNo?RmtNoValue="+rmtky, true);
  xhttpRmtky.send();  
} //--------3--------------------------------------------------------------


  
function checKey(){

  var RemoteNumber;
  var KeyNumber;
  var ProgGroupNumber;
  var xhttpCheckey = new XMLHttpRequest();
  xhttpCheckey.onreadystatechange = function() {
/*    if (this.readyState == 4 && this.status == 200) {
      RemoteNumber=document.getElementById("RemoteNoID").value;
    }*/
  };
    //document.getElementById("RemoteNoID").value =+1;
      RemoteNumber=document.getElementById("RemoteNoID").value;
      KeyNumber=document.getElementById("KeyNoID").value;
      ProgGroupNumber=document.getElementById("ProgGroupID").value;
      var rmtky=
                parseInt(RemoteNumber *0x10000)+
                parseInt(KeyNumber*0x100)+
                parseInt(ProgGroupNumber);
//      alert(rmtky);
  xhttpCheckey.open("POST", "/checkey?RmtNoCheck="+rmtky, true);
  xhttpCheckey.send();  
  
}