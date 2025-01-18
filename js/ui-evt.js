const lang = document.body.lang;

$(document).ready(function () {
  readA2D();
  readConnectionStatus();
  readHardwareInfo();
  readSDInfo();
  getConfig("network", function (data) {
    setNetworkSetting(data);
  });
});

$("#btnSaveNetSetting").click(async function (e) {
  const netConfig = makeWifiConfigJSON();
  sendNetworkSetting(netConfig, lang)
    .then((r) => {
      document.getElementById("btnCancelNetSetting").click();
    })
    .catch((error) => {});
  /*
  
  const isIpValid = isValidIP(netConfig.ip);
  const isValidPrimaryDNS = isValidIP(netConfig.pd);
  const isValidGateway = isValidIP(netConfig.gw);
  const isValidSubnetMask = isValidIP(netConfig.sm);
  const isValidSecondaryDNS = isValidIP(netConfig.sd);

  const isValidSSID = isValidSSIDName(netConfig.wn);

  if (
    !isIpValid ||
    !isValidGateway ||
    !isValidPrimaryDNS ||
    !isValidSubnetMask ||
    !isValidSecondaryDNS
  ) {
    $("#error_panel").css({ display: "block" });
    $("#error_panel").text(CS(lang).network_setting_error);
  } else {
    $("#error_panel").css({ display: "none" });
    sendNetworkSetting(netConfig, lang)
      .then((r) => {
        document.getElementById("btnCancelNetSetting").click();
      })
      .catch((error) => {});
  }
      */
});

$("#ipAddress").on("keypress", function (evt) {
  //console.log(evt);
  if ((evt.key < "0" || evt.key > "9") && evt.key !== ".") {
    event.preventDefault();
  }
});

function isValidIP(ipaddress) {
  return /^(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)\.(25[0-5]|2[0-4][0-9]|[1]?[1-9][1-9]?)$/.test(
    ipaddress
  );
}

function isValidSSIDName(ssidName) {
  return /^[!#;].|[+\[\]/"\t\s].*$/.test(ssidName);
}

function setNetworkSetting(netConfig) {
  $("#ipAddress").val(netConfig.ip);
  $("#subnetMask").val(netConfig.sm);
  $("#gateway").val(netConfig.gw);
  $("#primaryDNS").val(netConfig.pd);
  $("#secondaryDNS").val(netConfig.sd);

  $("#wifi_name").val(netConfig.wn);
  $("#wifi_pass").val(netConfig.wp);

  if (netConfig.mode === 0) {
    $("#rbHotspot").attr("checked", false);
  } else {
    $("#rbHotspot").attr("checked", true);
  }

  if (netConfig.dhcp === 0) {
    $("#rbDHCP").attr("checked", false);
  } else {
    $("#rbDHCP").attr("checked", true);
    $("#ipAddress").attr("disabled", "");
    $("#subnetMask").attr("disabled", "");
    $("#gateway").attr("disabled", "");
    $("#primaryDNS").attr("disabled", "");
    $("#secondaryDNS").attr("disabled", "");
  }
  //$("#rbDHCP").attr("checked", true).trigger("click");
}
