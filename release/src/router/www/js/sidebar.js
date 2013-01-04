// grab what the current firmware supports
var rcSupport = '<% nvram_get("rc_support"); %>';

var menuContent = "";

menuContent += "<li class='nav-header'>Device</li>";
menuContent += "<li><a href='/b_index.asp'>Status</a></li>";
menuContent += "<li><a href='/b_admin.asp'>Administration</a></li>";
menuContent += "<li><a href='/b_logs.asp'>Logs</a></li>";

// Dsl menu
menuContent += "<li class='nav-header'>Dsl</li>";

// put here Internet Connection, DMZ, DDNS, NAT Passthrough
menuContent += "<li><a href='/b_dsl_settins.asp'>Dsl Settings</a></li>";
menuContent += "<li><a href='/b_dsl_porttrigger.asp'>Port Trigger</a></li>";
menuContent += "<li><a href='/b_dsl_portforwarding.asp'>Port Forwarding</a></li>";


// Lan menu
menuContent += "<li class='nav-header'>Lan</li>";

// put here LAN IP, DHCP Server, IPTV
menuContent += "<li><a href='/b_lan_ip.asp'>IP Settings</a></li>";
menuContent += "<li><a href='/b_lan_route.asp'>Route</a></li>";


// Wireless menu
menuContent += "<li class='nav-header'>WiFi</li>";

// put here General 2.4GHz, Wireless MAC Filter 2.4GHz, Radius Setting 5GHz, Professional 2.4GHz
menuContent += "<li><a href='/b_wifi_24.asp'>2.4 GHz Settings</a></li>";
// put here General 5GHz, Wireless MAC Filter 5GHz, Radius Setting 5GHz, Professional 5GHz
menuContent += "<li><a href='/b_wifi_5.asp'>5 GHz Settings</a></li>";
menuContent += "<li><a href='/b_wifi_wps.asp'>WPS</a></li>";


// IPv6 menu
if(rcSupport.search("ipv6") != -1){
    menuContent += "<li class='nav-header'>IPv6</li>";

    menuContent += "<li><a href='/b_ipv6.asp'>IPv6 Settings</a></li>";
}


// VPN menu
menuContent += "<li class='nav-header'>VPN</li>";

menuContent += "<li><a href='/b_vpn.asp'>VPN Settings</a></li>";


// Firewall menu
menuContent += "<li class='nav-header'>Firewall</li>";

menuContent += "<li><a href='/b_firewall_settings.asp'>Firewall Settings</a></li>";
menuContent += "<li><a href='/b_firewall_url_filter.asp'>URL Filter</a></li>";
menuContent += "<li><a href='/b_firewall_keyword_filter.asp'>Keyword Filter</a></li>";
menuContent += "<li><a href='/b_firewall_network_services_filter.asp'>Network Services Filter</a></li>";


// USB menu
menuContent += "<li class='nav-header'>Usb</li>";

menuContent += "<li><a href='/b_usb_aidisk.asp'>AiDisk</a></li>";
menuContent += "<li><a href='/b_usb_server_center.asp'>Server Center</a></li>";
menuContent += "<li><a href='/b_usb_network_printer.asp'>Network Printer</a></li>";
menuContent += "<li><a href='/b_usb_3g_modem.asp'>3G/4G Modem</a></li>";
menuContent += "<li><a href='/b_usb_download_master.asp'>Download Master</a></li>";
menuContent += "<li><a href='/b_usb_media_server.asp'>Media Server</a></li>";

// Services menu
menuContent += "<li class='nav-header'>Services</li>";

menuContent += "<li><a href='/b_guest_network.asp'>Guest Network</a></li>";
menuContent += "<li><a href='/b_traffic_manager.asp'>Traffic Manager</a></li>";
menuContent += "<li><a href='/b_parental_controlasp'>Parental Control</a></li>";


// Modal
menuContent += "<div id='myModal' class='modal hide fade' tabindex='-1' role='dialog' aria-labelledby='myModalLabel' aria-hidden='true'>";
menuContent += "<div class='modal-header'>";
menuContent += "<button type='button' class='close' data-dismiss='modal' aria-hidden='true'>&times;</button>";
menuContent += "<h3 id='myModalLabel'></h3>";
menuContent += "</div>";
menuContent += "<div class='modal-body'>";
menuContent += "<h4>Please wait...</h4>";
menuContent += "<div class='progress progress-striped active'> <div class='bar' id='progress' style='width: 0%;'></div> </div>";
menuContent += "</div></div>";

// init interval time for modals
var intervalProgressTime = 0;
// init starting progress time
var startProgressTime = 0;

// reboot function
function reboot(){
    // set value for label
    $('#myModalLabel').val('Rebooting');
    // request dsl-n55u to rebbot
    $.post('apply.cgi', {
            'action_mode':'reboot'
            },function(data){
            });
    // set value for reboot
    intervalProgressTime = 75;
    // set value for starting upgrade progress
    startProgressTime = parseInt(new Date().getTime()/1000);
    // start countdown process
    setInterval(updateProgress, 1000);
}

// update progress function
function updateProgress(){
    // calculate progress
    var progress = parseInt(parseInt(new Date().getTime()/1000)/(intervalProgressTime+startProgressTime) * 100);

    // if progress > 100 reload page
    if(progress > 100)
        location.href = location.href;
    else { // otherwise update progress bar
        $('#progress').css('width',progress+'%')
    }
}


// sidebar function
function showSidebar(){
    // populate sidebar
    $('#sidebarmenu').html(menuContent);
    // add reboot button
    $('#sidebarmenu').append('<hr/><a onclick="javascript:reboot();" data-toggle="modal" href="#myModal" class="btn btn-large btn-danger" style="width:250px;">Reboot</a>');
    // auto set current page with class='active'
    $( "#sidebarmenu li" ).each(function( index ) {

        // if this li contains a link
        if($(this).find('a')[0]){
            // check if it is current link in nav bar
            if(location.href.search($(this).find('a')[0]) != -1)
                $(this).addClass('active');
        }
    });
}
