
// Load Config file from Local file Reside in OS File Structure 
// Alireza Salehi

function loadSetting(){
    const xhttp = new XMLHttpRequest();
    xhttp.onload  = function(){
        document.getElementById("demo").innerHTML = 
        this.responseText;
    }
    xhttp.open("GET", "ajax_info.txt");
    xhttp.send();
    
}


// Change UI WIFI Panel to disabled Status and Vice Versa 
// Alireza Salehi

async function rbChangeWIFIMode() {
   
    var ipAddress = document.getElementById("ipAddress");
    var gateway = document.getElementById("gateway");
    var subnetMask = document.getElementById("subnetMask");
    var primaryDNS = document.getElementById("primaryDNS");
    var secondaryDNS = document.getElementById("secondaryDNS");

    const wifiMode = document.getElementById("rbHotspot");
    //console.log(wifiMode.attributes);

    if(wifiMode.checked){
        ipAddress.setAttribute("disabled", "");
        gateway.setAttribute("disabled", "");
        subnetMask.setAttribute("disabled", "");
        primaryDNS.setAttribute("disabled", "");
        secondaryDNS.setAttribute("disabled", "");
  
    }else{
        ipAddress.removeAttribute("disabled");
        gateway.removeAttribute("disabled");
        subnetMask.removeAttribute("disabled");
        primaryDNS.removeAttribute("disabled");
        secondaryDNS.removeAttribute("disabled");

      

    }
}

// Collect All Wifi Required Information and Credentials and put it to JSON format 
// Alireza Salehi 
 function makeWifiConfigJSON(){

    var ip = document.getElementById("ipAddress").value;
    var gateway = document.getElementById("gateway").value;
    var subnetMask = document.getElementById("subnetMask").value;
    var primaryDNS = document.getElementById("primaryDNS").value;
    var secondaryDNS = document.getElementById("secondaryDNS").value;

    var wifi_name = document.getElementById("wifi_name").value;
    var wifiPass = document.getElementById("wifi_pass").value;

    const JSONConfig = {
        ip : ip,
        gw : gateway,
        sm : subnetMask,
        pd : primaryDNS,
        sd : secondaryDNS,
        wn : wifi_name,
        wp : wifiPass
    };
   

    return JSONConfig;

}


