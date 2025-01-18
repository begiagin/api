"use strict";

// in this file we should collect all information and function
// for gathering required data and send to ESP8266 Microcontroller
function sendNetworkSetting(netJson, lang) {
  return regeneratorRuntime.async(function sendNetworkSetting$(_context) {
    while (1) {
      switch (_context.prev = _context.next) {
        case 0:
          $.ajax({
            url: "/change-net_config",
            type: "POST",
            data: netJson,
            contentType: "application/json",
            success: function success(data) {
              //alert(CS(lang).network_save_cfg_success);
              return true;
            },
            error: function error(xhRequest, ErrorText, thrownError) {
              alert(ErrorText);
              return false;
            }
          });

        case 1:
        case "end":
          return _context.stop();
      }
    }
  });
}

function getConfig(section) {
  var ret_vale = {};
  $.ajax({
    url: "/get-config",
    // Replace with your URL
    type: "GET",
    dataType: "json",
    async: false // Expect a JSON respons

  }).done(function (response) {
    ret_vale = response;
  }).error(function (error) {
    ret_vale = null;
  });
  return ret_vale;
}