// in this file we should collect all information and function
// for gathering required data and send to ESP8266 Microcontroller

async function sendNetworkSetting(netJson, lang) {
  $.ajax({
    url: "/change-net_config",
    type: "POST",
    data: netJson,
    contentType: "application/json",
    success: function (data) {
      alert(CS(lang).network_save_cfg_success);
      return true;
    },
    error: function (xhRequest, ErrorText, thrownError) {
      alert(CS(lang).network_save_cfg_fail);
      return false;
    },
  });
}

function getConfig(section, fnToHandle) {
  $.ajax({
    url: "/get-config", // Replace with your URL
    type: "GET",
    dataType: "json", // Expect a JSON response
    success: function (response) {
      fnToHandle(response);
    },
    error: function (xhr, status, error) {
      console.error("Error:", error);
    },
  });
}
