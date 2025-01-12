
function loadSetting(){
    const xhttp = new XMLHttpRequest();
    xhttp.onload  = function(){
        document.getElementById("demo").innerHTML = 
        this.responseText;
    }
    xhttp.open("GET", "ajax_info.txt");
    xhttp.send();
}

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