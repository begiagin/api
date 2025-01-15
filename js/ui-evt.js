

const lang = document.body.lang;


$(document).ready(function () {
   readA2D();
   readConnectionStatus();
   readHardwareInfo();
   readSDInfo();
 });

$('#btnSaveNetSetting').click(async function (e) {

   const netConfig = makeWifiConfigJSON();
   const isIpValid = isValidIP(netConfig.ip);
   const isValidPrimaryDNS = isValidIP(netConfig.pd);
   const isValidGateway = isValidIP(netConfig.gw);
   const isValidSubnetMask = isValidIP(netConfig.sm);
   const isValidSecondaryDNS = isValidIP(netConfig.sd);


   const isValidSSID = isValidSSIDName(netConfig.wn);

   if (!isIpValid || !isValidGateway || !isValidPrimaryDNS || !isValidSubnetMask || !isValidSecondaryDNS) {
      $("#error_panel").css({'display': 'block'});
      $('#error_panel').text(CS(lang).network_setting_error);

   } else {
      $("#error_panel").css({'display': 'none'});
      sendNetworkSetting(netConfig, lang).then(r => {document.getElementById("btnCancelNetSetting").click();}).catch(error => {});

   }
});

$('#ipAddress').on("keypress", function(evt){
   //console.log(evt);
   if ((evt.key < '0' || evt.key > '9') && evt.key !== '.') {
       event.preventDefault()
     }
});


function isValidIP(ipaddress) {  
   return /^(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)$/.test(ipaddress);

} 

function isValidSSIDName(ssidName){
   return /^[!#;].|[+\[\]/"\t\s].*$/.test(ssidName);
}