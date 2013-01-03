var footerContent = "";

footerContent += '<p><i class=\'icon-off\'></i> DSL-N55U Bender Firmware v.<% nvram_get("firmver"); %>.<% nvram_get("buildno"); %></p>';

function showFooter(){
    $('#footer').html(footerContent);
}
