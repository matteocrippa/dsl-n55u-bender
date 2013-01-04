<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Device Status - DSL-N55U</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- stylesheets -->
    <link href="/css/bootstrap.min.css" rel="stylesheet">
    <style type="text/css">
      body {
        padding-top: 60px;
        padding-bottom: 40px;
      }
      .sidebar-nav {
        padding: 9px 0;
      }
    </style>

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

                <h2>Device Status</h2>
                <p>Uptime: <b id="boottime"></b></p>
                <hr />

                <h2>Dsl</h2>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Status</th>
                            <th>Internet IP</th>
                            <th>DDNS</th>
                            <th>Ping</th>
                        </tr>
                    </thead>
                    <tbody id="dslstatus">
                    </tbody>
                </table>
                <hr/>

                <h2>Lan</h2>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Router IP</th>
                            <th>Subnet Mask</th>
                            <th>DHCP</th>
                        </tr>
                    </thead>
                    <tbody id="lanstatus">
                    </tbody>
                </table>

                <hr/>

                <h2>Wireless</h2>
                <textarea id="wirelessLog" style="display:none;"><% nvram_dump("wlan11b_2g.log","wlan11b.sh"); %></textarea>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Radio</th>
                            <th>SSID</th>
                            <th>Status</th>
                            <th>Security</th>
                            <th>Channel</th>
                        </tr>
                    </thead>
                    <tbody id="wirelessinfo">
                    </tbody>
                </table>
            <hr />
        </div>
        <div class="row-fluid">
            <h3>Clients</h3>
            <textarea id="dhcpString" style="display:none;"><% nvram_dump("leases.log", "leases.sh"); %></textarea>
             <table class="table">
                    <thead>
                        <tr>
                            <th>IP</th>
                            <th>Name</th>
                            <th>MAC</th>
                            <th>Type</th>
                        </tr>
                    </thead>
                    <tbody id="clientsinfo">
                    </tbody>
                </table>

            <br />
            <h3>Usb</h3>
            <table class="table">
                    <thead>
                        <tr>
                            <th>USB Port</th>
                            <th>Name</th>
                            <th>Type</th>
                            <th></th>
                            <th></th>
                        </tr>
                    </thead>
                    <tbody id="usbinfo">
                    </tbody>
                </table>

        </div>
      </div>
  </div>
      <hr>

      <footer id="footer">
      </footer>

    </div>

    <!-- javascripts libs -->
    <script src="/js/jquery.min.js"></script>
    <script src="/js/bootstrap.min.js"></script>
    <!-- bender templates -->
    <script src="/js/sidebar.js"></script>
    <script src="/js/footer.js"></script>
    <!-- bender render pages -->
    <script src="/js/index.js"></script>
    <script>
        $(document).ready(function(){
                // dynamic layout
                showSidebar();
                showFooter();

                // dynamic contents
                populatePage();
        });
    </script>
  </body>
</html>
