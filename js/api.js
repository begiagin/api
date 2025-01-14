// in this file we should collect all information and function 
// for gathering required data and send to ESP8266 Microcontroller

async function sendNetworkSetting(netJson){
    $.ajax({                    
        url: "/chang-net_config",
        type: "POST",
        data: netJson,
        contentType: 'application/json',
        success: function (data) {
            alert(CS().network_save_cfg_success);
        },
        error: function (xhRequest, ErrorText, thrownError) {
            alert("Failed to process correctly, please try again");
        }
    });

}
