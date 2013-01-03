// grab what the current firmware supports
var rcSupport = '<% nvram_get("rc_support"); %>';

var menuContent = "";

menuContent += "<li class='nav-header'>Device</li>";
menuContent += "<li class='active'><a href='/'>Status</a></li>";
menuContent += "<li><a href=''>Administration</a></li>";
menuContent += "<li><a href=''>Logs</a></li>";

// Dsl menu
menuContent += "<li class='nav-header'>Dsl</li>";

// put here Internet Connection, DMZ, DDNS, NAT Passthrough
menuContent += "<li><a href=''>Dsl Settings</a></li>";
menuContent += "<li><a href=''>Port Trigger</a></li>";
menuContent += "<li><a href=''>Port Forwarding</a></li>";


// Lan menu
menuContent += "<li class='nav-header'>Lan</li>";

// put here LAN IP, DHCP Server, IPTV
menuContent += "<li><a href=''>IP Settings</a></li>";
menuContent += "<li><a href=''>Route</a></li>";


// Wireless menu
menuContent += "<li class='nav-header'>Wireless</li>";

// put here General 2.4GHz, Wireless MAC Filter 2.4GHz, Radius Setting 5GHz, Professional 2.4GHz
menuContent += "<li><a href=''>2.4 GHz Settings</a></li>";
// put here General 5GHz, Wireless MAC Filter 5GHz, Radius Setting 5GHz, Professional 5GHz
menuContent += "<li><a href=''>5 GHz Settings</a></li>";
menuContent += "<li><a href=''>WPS</a></li>";


// IPv6 menu
if(rcSupport.search("ipv6") != -1){
    menuContent += "<li class='nav-header'>IPv6</li>";

    menuContent += "<li><a href=''>IPv6 Settings</a></li>";
}


// VPN menu
menuContent += "<li class='nav-header'>VPN</li>";

menuContent += "<li><a href=''>VPN Settings</a></li>";


// Firewall menu
menuContent += "<li class='nav-header'>Firewall</li>";

menuContent += "<li><a href=''>Firewall Settings</a></li>";
menuContent += "<li><a href=''>URL Filter</a></li>";
menuContent += "<li><a href=''>Keyword Filter</a></li>";
menuContent += "<li><a href=''>Network Services Filter</a></li>";


// USB menu
menuContent += "<li class='nav-header'>Usb</li>";

menuContent += "<li><a href=''>AiDisk</a></li>";
menuContent += "<li><a href=''>Server Center</a></li>";
menuContent += "<li><a href=''>Network Printer</a></li>";
menuContent += "<li><a href=''>3G/4G Modem</a></li>";
menuContent += "<li><a href=''>Download Master</a></li>";
menuContent += "<li><a href=''>Media Server</a></li>";

// Services menu
menuContent += "<li class='nav-header'>Services</li>";

menuContent += "<li><a href=''>Guest Network</a></li>";
menuContent += "<li><a href=''>Traffic Manager</a></li>";
menuContent += "<li><a href=''>Parental Control</a></li>";

function showSidebar(){
    $('#sidebarmenu').html(menuContent);
}
