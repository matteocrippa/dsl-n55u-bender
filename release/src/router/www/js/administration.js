function upgradeFirmware(){

    // set new title for modal
    $('#myModalLabel').text('Upgrading Firmware');

    // start uploading new firmware
    $('#firmware').ajaxfileupload({
            'action': 'upgrade.cgi',
            },
            'onComplete': function(response){
                console.log(response);
            }
    });

    // set amount of time for this operation
    intervalProgressTime = totalAmountProgressTime = 135;

    // start countdown
    setInterval(updateProgress, 1000);

}
