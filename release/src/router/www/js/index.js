var uptimeStr = "<% uptime(); %>";
var boottime = parseInt(uptimeStr.substring(32,42));

// generate boot time in human readable way
function showBootTime(){

    var days = Math.floor(boottime / (60*60*24));
    var hours = Math.floor((boottime / 3600) % 24);
    var minutes = Math.floor(boottime % 3600 / 60);
    var seconds = Math.floor(boottime % 60);

    $('#boottime').html(days+ ' days , '+hours+' hours, '+minutes+' minutes, '+seconds+' seconds');

    boottime += 1;

    setTimeout("showBootTime()",1000);
}

// render dsl information vars
<% wanlink(); %>
var ddnsName = decodeURIComponent('<% nvram_char_to_ascii("", "ddns_hostname_x"); %>');


// FOR TESTING ONLY
/*function wanlink_status() { return 1;}
function wanlink_statusstr() { return 'Connected';}
function wanlink_type() { return 'pppoe';}
function wanlink_ipaddr() { return '82.60.5.244';}
function wanlink_netmask() { return '255.255.255.255';}
function wanlink_gateway() { return '192.168.100.1';}
function wanlink_dns() { return '8.8.8.8 8.8.4.4';}
function wanlink_lease() { return 0;}
function wanlink_expires() { return 0;}
function is_private_subnet() { return '0';}
function wanlink_xtype() { return '';}
function wanlink_xipaddr() { return '169.254.89.130';}
function wanlink_xnetmask() { return '255.255.0.0';}
function wanlink_xgateway() { return '';}
function wanlink_xdns() { return '';}
function wanlink_xlease() { return 0;}
function wanlink_xexpires() { return 0;}
var ddnsName = decodeURIComponent('hwlan%2Easuscomm%2Ecom');*/
// REMOVE

// generate dsl info
function showDslInfo(){

    var dslInfo = "";

    // check if dsl is connected
    if(wanlink_status() == 1){

        dslInfo += '<tr class="success">';
        dslInfo += '<td>Connected</td>';
        dslInfo += '<td>'+wanlink_ipaddr()+'</td>';
        dslInfo += '<td><a href="http://'+ddnsName+'">'+ddnsName+'</a></td>';
        dslInfo += '<td>ping?</td>';
        dslInfo += '</tr>';

    }else{ // if not connected
        dslInfo += '<tr class="error">';
        dslInfo += '<td>Not Connected</td>';
        dslInfo += '<td></td>';
        dslInfo += '<td></td>';
        dslInfo += '<td></td>';
        dslInfo += '</tr>';
    }

    // populate html
    $('#dslstatus').html(dslInfo);
}

// generate lan info
function showLanInfo(){
    var lanInfo = "";

    var isDhcp = '<% nvram_get("dhcp_enable_x"); %>';

    if(isDhcp)
        isDhcp = 'Active';
    else
        isDhcp = 'Disabled';

    lanInfo += '<tr class="info">';
    lanInfo += '<td><% nvram_get("lan_ipaddr"); %></td>';
    lanInfo += '<td><% nvram_get("lan_netmask_rt"); %></td>';
    lanInfo += '<td>'+isDhcp+'</td>';
    lanInfo += '</tr>';

    // populate html
    $('#lanstatus').html(lanInfo);
}

// render wireless vars
var ssid_status_2g =  decodeURIComponent('<% nvram_char_to_ascii("WLANConfig11b", "wl0_ssid"); %>');
var ssid_status_5g =  decodeURIComponent('<% nvram_char_to_ascii("WLANConfig11b", "wl1_ssid"); %>');


// FOR TESTING ONLY
//var ssid_status_2g = decodeURIComponent('HWLAN');
//var ssid_status_5g = decodeURIComponent('HWLAN%5F5G');
// REMOVE

// generate wireless info
function showWirelessInfo(){
    var wirelessInfo = "";

    // get wireless log (both 2.4GHz and 5GHz)
    var wirelessLog = $('#wirelessLog').val();
    var is2GEnabled = wirelessLog.search('2.4 GHz radio is disabled');
    var is5GEnabled = wirelessLog.search('5 GHz radio is disabled');

    // retrieve wireless security
    var authMode2G = '<% nvram_get("wl0_auth_mode_x"); %>';
    var wirelessSecurity2G = "None";

    if(authMode2G == 'shared')
        wirelessSecurity2G = "Shared Key";
    else if(authMode2G == 'psk')
        wirelessSecurity2G = "WPA-Personal";
    else if(authMode2G == 'psk2')
        wirelessSecurity2G = "WPA2-Personal";
    else if(authMode2G == 'pskpsk2')
        wirelessSecurity2G = "WPA-Auto-Personal";
    else if(authMode2G == 'wpa')
        wirelessSecurity2G = "WPA-Enterprise";
    else if(authMode2G == 'wpa2')
        wirelessSecurity2G = "WPA2-Enterprise";
    else if(authMode2G == 'wpawpa2')
        wirelessSecurity2G = "WPA-Auto-Enterprise";
    else if(authMode2G == 'radius')
        wirelessSecurity2G = "Radius with 802.1x";

    // if security is not open
    if(wirelessSecurity2G != "None")
        wirelessSecurity2G += " <i class='icon-lock'></i>";


    // 2.4 GHz
    wirelessInfo +="<tr class='";
    if(is2GEnabled != -1)
        wirelessInfo +="error";
    else
        wirelessInfo +="success";
    wirelessInfo +="'>";
    wirelessInfo +="<td>2.4 GHz</td>";
    wirelessInfo +="<td>"+ssid_status_2g+"</td>";
    wirelessInfo +="<td><div class='btn-group'>";
    // WiFi 2.4GHz is disabled
    if(is2GEnabled !=1){

        wirelessInfo +="<a class='btn btn-danger dropdown-toggle' data-toggle='dropdown' href='#'>Disabled<span class='caret'></span></a>";
        wirelessInfo +="<ul class='dropdown-menu'>";
        wirelessInfo +="<li><a onclick='javascript:enableWifi(0);' data-toggle='modal' href='#myModal'>Enable</a></li>";
        wirelessInfo +="</ul>";

    }else{ // otherwise

        wirelessInfo +="<a class='btn btn-success dropdown-toggle' data-toggle='dropdown' href='#'>Active<span class='caret'></span></a>";
        wirelessInfo +="<ul class='dropdown-menu'>";
        wirelessInfo +="<li><a onclick='javascript:disableWifi(0);' data-toggle='modal' href='#myModal'>Disable</a></li>";
        wirelessInfo +="</ul>";

    }
    wirelessInfo +="</div></td>"

    if(is2GEnabled != -1)
        wirelessInfo +="<td>Disabled</td>";
    else
        wirelessInfo +="<td>Active</td>";
    w
    wirelessInfo +="<td>"+wirelessSecurity2G+"</td>";
    wirelessInfo +="<td><% nvram_get("wl0_channel"); %></td>";
    wirelessInfo +="</tr>";


    // retrieve wireless security
    var authMode5G = '<% nvram_get("wl1_auth_mode_x"); %>';
    var wirelessSecurity5G = "None";

    if(authMode5G == 'shared')
        wirelessSecurity5G = "Shared Key";
    else if(authMode5G == 'psk')
        wirelessSecurity5G = "WPA-Personal";
    else if(authMode5G == 'psk2')
        wirelessSecurity5G = "WPA2-Personal";
    else if(authMode5G == 'pskpsk2')
        wirelessSecurity5G = "WPA-Auto-Personal";
    else if(authMode5G == 'wpa')
        wirelessSecurity5G = "WPA-Enterprise";
    else if(authMode5G == 'wpa2')
        wirelessSecurity5G = "WPA2-Enterprise";
    else if(authMode5G == 'wpawpa2')
        wirelessSecurity5G = "WPA-Auto-Enterprise";
    else if(authMode5G == 'radius')
        wirelessSecurity5G = "Radius with 802.1x";

    // if security is not open
    if(wirelessSecurity5G != "None")
        wirelessSecurity5G += " <i class='icon-lock'></i>";

    // 5 GHz
    wirelessInfo +="<tr class='";
    if(is5GEnabled != -1)
        wirelessInfo +="error";
    else
        wirelessInfo +="success";
    wirelessInfo +="'>";
    wirelessInfo +="<td>5 GHz</td>";
    wirelessInfo +="<td>"+ssid_status_5g+"</td>";

    wirelessInfo +="<td><div class='btn-group'>";
    // WiFi 5GHz is disabled
    if(is5GEnabled !=1){

        wirelessInfo +="<a class='btn btn-danger dropdown-toggle' data-toggle='dropdown' href='#'>Disabled<span class='caret'></span></a>";
        wirelessInfo +="<ul class='dropdown-menu'>";
        wirelessInfo +="<li><a onclick='javascript:enableWifi(1);' data-toggle='modal' href='#myModal'>Enable</a></li>";
        wirelessInfo +="</ul>";

    }else{ // otherwise

        wirelessInfo +="<a class='btn btn-success dropdown-toggle' data-toggle='dropdown' href='#'>Active<span class='caret'></span></a>";
        wirelessInfo +="<ul class='dropdown-menu'>";
        wirelessInfo +="<li><a onclick='javascript:disableWifi(1);' data-toggle='modal' href='#myModal'>Disable</a></li>";
        wirelessInfo +="</ul>";

    }
    wirelessInfo +="</div></td>"

    wirelessInfo +="<td>"+wirelessSecurity5G+"</td>";
    wirelessInfo +="<td><% nvram_get("wl1_channel"); %></td>";
    wirelessInfo +="</tr>";

    // populate html
    $('#wirelessinfo').html(wirelessInfo);

}

// declare activate wifi function
function enableWifi(what){
    // set value for modal label
    $('#myModalLabel').text('Wireless Rebooting');
    // request via POST to activate wifi
    $.post('start_apply.htm',
            {
                'action_mode':'apply',
                'action_script':'restart_wireless',
                'action_wait':'3',
                'wl_unit':what,
                'wl_subunit':'-1',
                'wl_radio':'1',
                'wl_radio_date_x_Mon':'on',
                'wl_radio_date_x_Tue':'on',
                'wl_radio_date_x_Wed':'on',
                'wl_radio_date_x_Thu':'on',
                'wl_radio_date_x_Fri':'on',
                'wl_radio_date_x_Sat':'on',
                'wl_radio_date_x_Sun':'on',
                'wl_radio_time_x_starthour':'00',
                'wl_radio_time_x_startmin':'00',
                'wl_radio_time_x_endhour':'23',
                'wl_radio_time_x_endmin':'59',
                'wl_radio_time2_x_starthour':'00',
                'wl_radio_time2_x_startmin':'00',
                'wl_radio_time2_x_endhour':'23',
                'wl_radio_time2_x_endmin':'59',
                'wl_radio_date_x':'1111111',
                'wl_radio_time_x':'00002359',
                'wl_radio_time2_x':'00002359'
            },function(data){  });

    // set value for reboot
    intervalProgressTime = totalAmountProgressTime = 32;
    // start countdown process
    setInterval(updateProgress, 1000);
}

// declare deactivate wifi function
function disableWifi(what){

    // set value for modal label
    $('#myModalLabel').text('Wireless Rebooting');
    // request via POST to activate wifi
    $.post('start_apply.htm',
            {
                'action_mode':'apply',
                'action_script':'restart_wireless',
                'action_wait':'3',
                'wl_unit':what,
                'wl_subunit':'-1',
                'wl_radio':'1',
                'wl_radio_date_x_Fri':'on',
                'wl_radio_time_x_starthour':'04',
                'wl_radio_time_x_startmin':'00',
                'wl_radio_time_x_endhour':'04',
                'wl_radio_time_x_endmin':'05',
                'wl_radio_date_x':'0000010',
                'wl_radio_time_x':'0400405',
            },function(data){  });

    // set value for reboot
    intervalProgressTime = totalAmountProgressTime = 32;
    // start countdown process
    setInterval(updateProgress, 1000);
}


// generate clients info
function showClients(){
    // retrieve the latest list of connected devices as string
    var clientString = '<% get_client_detail_info(); %>';
    // convert it to an array
    var clientArray = clientString.split(",");

    // contains information about dhcp devices
    var dhcpString = $('#dhcpString').val();
    // convert it to array
    var dhcpArray = dhcpString.split("\n");
    // remove the headers
    dhcpArray.splice(0,1);

    // init clientsInfo
    var clientsInfo = "None";

    // match ip addresses in client string
    var validClients = clientString.match(/\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b/g);

    // if we have at least a connected client
    if(validClients){

        // but if clientArray.length > 0 reset it
        if(clientArray.length > 0)
            clientsInfo = "";

        //loop inside connected devices
        for(var i=0;i<clientArray.length;i++){
            //split client info in items
            var fields = clientArray[i].split(">");

            // connection type init as static
            var connType = "Static";

            // connection device name
            var deviceName = "";

            // loop inside dhcps to find out if it is dhcp or static
            for(var j=0;j<dhcpArray.length;j++){

                // grab out ip related to macaddress
                var ipAddress = dhcpArray[j].match(/\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b/g);

                // check if there any array
                if(ipAddress){

                    // if we found the ip inside dhcp list
                    if(ipAddress[0] == fields[2]){
                        // set dhcp
                        connType = "DHCP";

                        // split dhcp fields into tokens
                        var dhcpFields = dhcpArray[j].split(" ");

                        // set deviceName
                        deviceName = dhcpFields[dhcpFields.length-1];

                        continue;
                    }


                }

            }

            clientsInfo += "<tr class='info'>";
            clientsInfo += "<td>"+fields[2]+"</td>";
            clientsInfo += "<td>"+deviceName+"</td>";
            clientsInfo += "<td>"+fields[3]+"</td>";
            clientsInfo += "<td>"+connType+"</td>";
            clientsInfo += "</tr>";
        }

    }

    $('#clientsinfo').html(clientsInfo);
}

// generate usb info
function showUsb(){
    var usbInfo = "";

    // usb 1 info
    var usb1_type = '<% nvram_get("usb_path1"); %>';
    var usb1_object = '<% nvram_get("usb_path1_manufacturer"); %> <% nvram_get("usb_path1_product"); %>';
    var usb1_removed = '<% nvram_get("usb_path1_removed"); %>';

    // replace usb type with correct icon
    if(usb1_type == "storage")
        usb1_type = "<i class='icon-hdd'></i>";
    else if(usb1_type == "printer")
        usb1_type = "<i class='icon-print'></i>";
    else if(usb1_type == "modem")
        usb1_type = "<i class='icon-signal'></i>";
    else
        usb1_type = "";

    // usb 2 info
    var usb2_type = '<% nvram_get("usb_path2"); %>';
    var usb2_object = '<% nvram_get("usb_path2_manufacturer"); %> <% nvram_get("usb_path2_product"); %>';
    var usb2_removed = '<% nvram_get("usb_path2_removed"); %>';

    // replace usb type with correct icon
    if(usb2_type == "storage")
        usb2_type = "<i class='icon-hdd'></i>";
    else if(usb2_type == "printer")
        usb2_type = "<i class='icon-print'></i>";
    else if(usb2_type == "modem")
        usb2_type = "<i class='icon-signal'></i>";
    else
        usb2_type = "";

    // usb 1 slot
    // if there is something attached
    if(usb1_type.length > 0){

        usbInfo += "<tr class='";
        // check if is mounted
        if(parseInt(usb1_removed) == 0)
            usbInfo += "warning";
        else
            usbInfo += "success";
        usbInfo += "'>";
        usbInfo += "<td>1</td>";
        usbInfo += "<td>"+usb1_object+"</td>";
        usbInfo += "<td>"+usb1_type+"</td>";
        usbInfo += "</tr>";

    }

    // usb 2 slot
    // if there is something attached
    if(usb2_type.length > 0){

        usbInfo += "<tr class='";

        // check if is mounted
        if(parseInt(usb2_removed) == 0)
            usbInfo += "warning";
        else
            usbInfo += "success";

        usbInfo += "'>";
        usbInfo += "<td>2</td>";
        usbInfo += "<td>"+usb2_object+"</td>";
        usbInfo += "<td>"+usb2_type+"</td>";
        usbInfo += "</tr>";
    }

        // populate html
    $('#usbinfo').html(usbInfo);
}

// master function to populate page contents
function populatePage(){
    // show boot time
    showBootTime();
    // show dsl info
    showDslInfo();
    // show lan info
    showLanInfo();
    // show wireless info
    showWirelessInfo();
    // show clients info
    showClients();
    // show usb info
    showUsb();
}
