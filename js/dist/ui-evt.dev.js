"use strict";

var lang = document.body.lang;
$(document).ready(function () {
  readA2D();
  readConnectionStatus();
  readHardwareInfo();
  readSDInfo();
  getConfig('network');
});
$('#btnSaveNetSetting').click(function _callee(e) {
  var netConfig, isIpValid, isValidPrimaryDNS, isValidGateway, isValidSubnetMask, isValidSecondaryDNS, isValidSSID;
  return regeneratorRuntime.async(function _callee$(_context) {
    while (1) {
      switch (_context.prev = _context.next) {
        case 0:
          netConfig = makeWifiConfigJSON();
          isIpValid = isValidIP(netConfig.ip);
          isValidPrimaryDNS = isValidIP(netConfig.pd);
          isValidGateway = isValidIP(netConfig.gw);
          isValidSubnetMask = isValidIP(netConfig.sm);
          isValidSecondaryDNS = isValidIP(netConfig.sd);
          isValidSSID = isValidSSIDName(netConfig.wn);

          if (!isIpValid || !isValidGateway || !isValidPrimaryDNS || !isValidSubnetMask || !isValidSecondaryDNS) {
            $("#error_panel").css({
              'display': 'block'
            });
            $('#error_panel').text(CS(lang).network_setting_error);
          } else {
            $("#error_panel").css({
              'display': 'none'
            });
            sendNetworkSetting(netConfig, lang).then(function (r) {
              document.getElementById("btnCancelNetSetting").click();
            })["catch"](function (error) {});
          }

        case 8:
        case "end":
          return _context.stop();
      }
    }
  });
});
$('#ipAddress').on("keypress", function (evt) {
  //console.log(evt);
  if ((evt.key < '0' || evt.key > '9') && evt.key !== '.') {
    event.preventDefault();
  }
});

function isValidIP(ipaddress) {
  return /^(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)$/.test(ipaddress);
}

function isValidSSIDName(ssidName) {
  return /^[!#;].|[+\[\]/"\t\s].*$/.test(ssidName);
}