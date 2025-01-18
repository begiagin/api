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
            contentType: 'application/json',
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
  return regeneratorRuntime.async(function getConfig$(_context2) {
    while (1) {
      switch (_context2.prev = _context2.next) {
        case 0:
          $.ajax({
            url: '/get-config?section=' + section,
            // Replace with your URL  
            type: 'GET',
            dataType: 'json',
            // Expect a JSON response  
            success: function success(response) {
              console.log('Success:', response);
            },
            error: function error(xhr, status, _error) {
              console.error('Error:', _error);
            }
          });

        case 1:
        case "end":
          return _context2.stop();
      }
    }
  });
}