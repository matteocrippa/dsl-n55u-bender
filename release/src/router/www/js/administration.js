// setup an input file to work in ajax
function applyAjaxFileUpload(element){
    $(element).ajaxfileupload({
        debug: true,
        action: 'upgrade.cgi',
        onChange: function(file){
            // show the modal
            $('#myModal').modal().show();
            // set the title
            $('#myModalLabel').text('Upgrading Firmware');
            // set the amount of time
            intervalProgressTime = totalAmountProgressTime = 195;
            // start countdown
            setInterval(updateProgress, 1000);
        }
    });
}

// reset password
function resetPassword(){

    // check password is valid
    if($('#pass').val() != $('#confirmpass').val()){
        // set the title
        $('#myModalLabel').text('Warning!');
        // set the subtitle
        $('#myModalDescription').text('Pay attention, password is different from confirm password');
        return;
    }

    // set the title
    $('#myModalLabel').text('Reset Password');
    // set the subtitle
    $('#myModalDescription').text('The new password takes effect after rebooting or the next time you log into the system.');
    // set the amount of time
    intervalProgressTime = totalAmountProgressTime = 35;
    // start countdown
    setInterval(updateProgress, 1000);
    // request POST to change password
    $.post('start_apply.htm',
            {
                'action_mode':'apply',
                'action_wait':'5',
                'action_script':'restart_time;restart_httpd',
                'http_passwd': $('#pass').val(),
                'http_passwd2': $('#pass').val(),
                'v_password2': $('#pass').val()

            },function(data){});
}

// restore factory settings
function restoreFactory(){

}

// restore settings



// populate page
function populatePage(){


}
