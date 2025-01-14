

const lang = document.body.lang;


$(document).ready(function () {
   readA2D();
   readConnectionStatus();
   readHardwareInfo();
   readSDInfo();
 });

$('#btnSaveNetSetting').click(function (e) { 

   const netConfig = makeWifiConfigJSON();
   var isIpValid = isValidIP(netConfig.ip);
   var isValidPrimaryDNS = isValidIP(netConfig.pd);
   var isValidGateway = isValidIP(netConfig.gw);
   var isValidSubnetMask = isValidIP(netConfig.sm);
   var isValidSecondaryDNS = isValidIP(netConfig.sd);


   var isVAlidSSID = isVAlidSSIDName(netConfig.wn);

   if(!isIpValid || !isValidGateway || !isValidPrimaryDNS || !isValidSubnetMask || !isValidSecondaryDNS){
      $("#error_panel").css({'display' : 'block'});
      $('#error_panel').text(CS(lang).network_setting_error);
      return;
   }
   else if(!isVAlidSSID) {
      $("#error_panel").css({'display' : 'block'});
      $('#error_panel').text(CS(lang).network_wifi_name);
      return;
   }else{
      $("#error_panel").css({'display' : 'none'});
      sendNetworkSetting(netConfig).then((value) => {
         console.log(value);
      })

      document.getElementById("btnSaveNetSetting").click();
   }
});

$('#ipAddress').on("keypress", function(evt){
   //console.log(evt);
   if ((evt.key < '0' || evt.key > '9') && evt.key !== '.') {
       event.preventDefault()
     }
});


function isValidIP(ipaddress) {  
   if (/^(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)$/.test(ipaddress)) {  
     return (true)  
   }   
   return (false) 
} 

function isVAlidSSIDName(ssidName){
   return /^[!#;].|[+\[\]/"\t\s].*$/.test(ssidName);
}