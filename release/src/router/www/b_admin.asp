<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Administration - DSL-N55U</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- stylesheets -->
    <link href="/css/bootstrap.min.css" rel="stylesheet">
    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="/js/html5.js"></script>
    <![endif]-->
  </head>

  <body>

    <div class="navbar navbar-inverse navbar-fixed-top">
      <div class="navbar-inner">
        <div class="container-fluid">
          <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </a>
          <a class="brand" href="/">DSL-N55U Administration Panel</a>
        </div>
      </div>
    </div>

    <div class="container-fluid">
      <div class="row-fluid">
        <!-- sidebar menu -->
        <div class="span3">
          <div class="well sidebar-nav">
            <ul class="nav nav-list" id="sidebarmenu">
            </ul>
          </div>
        </div>

        <!-- contents -->
        <div class="span9">
            <div class="row-fluid">

                <h2>Administrator Password</h2>
                <p>New password: <input type="text" name="password" id="pass" /></p>
                <p>Confirm new password: <input type="text" name="confirmpassword" id="confirmpass" /></p>
                <a href="#myModal" data-toggle="modal" class="btn btn-large" onclick="javascript:resetPassword();">Change Password</a>
                <hr />

                <h2>Firmware Upgrade</h2>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Service</th>
                            <th>Version</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>ADSL Driver</td>
                            <td><% nvram_dump("adsl/tc_fw_ver_short.txt",""); %></td>
                        </tr>
                        <tr>
                            <td>RAS</td>
                            <td><% nvram_dump("adsl/tc_ras_ver.txt",""); %></td>
                        </tr>
                        <tr>
                            <td>Firmware</td>
                            <td><% nvram_get("firmver"); %>.<% nvram_get("buildno"); %></td>
                        </tr>
                    </tbody>
                </table>
                <p>
                    <form action='' method='POST' enctype='multipart/form-data'>
                        <input type="file" name="file" id="firmware">
                        <a href="#myModal" data-toggle="modal" class="btn btn-large" onclick="javascript:upgradeFirmware();">Upgrade Firmware</a>
                    </form>
                </p>
                <hr/>

                <h2>Settings Management</h2>
                <a onclick="javascript:restoreFactory();" data-toggle="modal" href="#myModal" class="btn btn-large btn-danger" style="width:250px;">Restore Factory Default Settings</a><br/>
                <a onclick="javascript:saveSettings();" data-toggle="modal" href="#myModal" class="btn btn-large btn-danger" style="width:250px;">Save Settings</a><br/>
                <a onclick="javascript:restoreSettings();" data-toggle="modal" href="#myModal" class="btn btn-large btn-danger" style="width:250px;">Restore Settings</a> <input type="file" name="file" /><br/>
                <hr/>

                <h2>Connection Services</h2>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Service</th>
                            <th>Status</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>Telnet</td>
                            <td></td>
                        </tr>
                        <tr>
                            <td>External (via Internet) Administration</td>
                            <td>
                                <a class='btn btn-success dropdown-toggle' data-toggle='dropdown' href='#'>Active<span class='caret'></span></a>
                                <ul class='dropdown-menu'>
                                    <li><a onclick='javascript:disableWifi(0);' data-toggle='modal' href='#myModal'>Disable</a></li>
                                </ul>
                            </td>
                        </tr>
                    </tbody>
                </table>
                <hr/>
            </div>
        </div>
    </div>
    <hr>

    <footer id="footer">
    </footer>

    </div>

    <!-- javascripts libs -->
    <script src="/js/jquery.min.js"></script>
    <script src="/js/jquery.ajaxfileupload.js"></script>
    <script src="/js/bootstrap.min.js"></script>
    <!-- bender templates -->
    <script src="/js/sidebar.js"></script>
    <script src="/js/footer.js"></script>
    <!-- bender render pages -->
    <script src="/js/administration.js"></script>
    <script>

        $(document).ready(function(){
                // dynamic layout
                showSidebar();
                showFooter();

                // setup upload field to work in ajax mode
                applyAjaxFileUpload('input#firmware');

                // populate page content
                populatePage();
        });
    </script>
  </body>
</html>
