
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

    const ipAddress = document.getElementById("ipAddress");
    const gateway = document.getElementById("gateway");
    const subnetMask = document.getElementById("subnetMask");
    const primaryDNS = document.getElementById("primaryDNS");
    const secondaryDNS = document.getElementById("secondaryDNS");

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
 function makeWifiConfigJSON() {

     const ip = document.getElementById("ipAddress").value;
     const gateway = document.getElementById("gateway").value;
     const subnetMask = document.getElementById("subnetMask").value;
     const primaryDNS = document.getElementById("primaryDNS").value;
     const secondaryDNS = document.getElementById("secondaryDNS").value;
     const wifiMode = document.getElementById("rbHotspot");
     const wifi_name = document.getElementById("wifi_name").value;
     const wifiPass = document.getElementById("wifi_pass").value;
     const dhcpMode = document.getElementById("rbHotspot");

     return {
         mode: !!wifiMode.checked,
         dhcp: !!dhcpMode.checked,
         ip: ip,
         gw: gateway,
         sm: subnetMask,
         pd: primaryDNS,
         sd: secondaryDNS,
         wn: wifi_name,
         wp: wifiPass
     };


 }

$('#rbDHCP').change(function() {
    if ($(this).is(':checked')) {
         $("#ipAddress").attr("disabled", "");
         $("#subnetMask").attr("disabled", "");
        $("#gateway").attr("disabled", "");
        $("#primaryDNS").attr("disabled", "");
        $("#secondaryDNS").attr("disabled", "");


    } else {
        $("#ipAddress").removeAttr("disabled");
        $("#subnetMask").removeAttr("disabled");
        $("#gateway").removeAttr("disabled");
        $("#primaryDNS").removeAttr("disabled");
        $("#secondaryDNS").removeAttr("disabled");

    }
});




