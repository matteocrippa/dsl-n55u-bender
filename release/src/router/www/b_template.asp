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
          <!--div class="hero-unit">
            <h1>Hello, world!</h1>
            <p>This is a template for a simple marketing or informational website. It includes a large callout called the hero unit and three supporting pieces of content. Use it as a starting point to create something more unique.</p>
            <p><a class="btn btn-primary btn-large">Learn more &raquo;</a></p>
            </div-->
            <div class="row-fluid">

                <h2>Device Status</h2>
                <p>Uptime: <b id="boottime">365 days, 23 hours, 59 minutes and 59 seconds</b></p>
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
                    <tbody>
                        <tr class="success">
                            <td>Connected</td>
                            <td>1.1.1.1</td>
                            <td><a href="">test.asuscomm.com</a></td>
                            <td>51ms</td>
                        </tr>
                    </tbody>
                </table>
                <hr/>

                <h2>Lan</h2>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Router IP</th>
                            <th>Netmask</th>
                            <th>DHCP</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr class="info">
                            <td>192.168.0.1</td>
                            <td>255.255.255.0</td>
                            <td>Active</td>
                        </tr>
                    </tbody>
                </table>

                <hr/>

                <h2>Wireless</h2>
                <p><i class="icon-lock"></i> Security: <b>WPA2-Personal</b></p>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Radio</th>
                            <th>SSID</th>
                            <th>Status</th>
                            <th>Channel</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr class="success">
                            <td>2.4 GHz</td>
                            <td><a href="">Asus2G</a></td>
                            <td>Active</td>
                            <td>11</td>
                        </tr>
                        <tr class="error">
                            <td>5 GHz</td>
                            <td><a href="">Asus5G</a></td>
                            <td>Disabled</td>
                            <td></td>
                        </tr>
                    </tbody>
                </table>
            <hr />
        </div>
        <div class="row-fluid">
            <h3>Clients</h3>
             <table class="table">
                    <thead>
                        <tr>
                            <th>IP</th>
                            <th>Name</th>
                            <th>MAC</th>
                            <th>Type</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr class="info">
                            <td>192.168.0.2</td>
                            <td>Device #1</td>
                            <td>FF:FF:FF:FF</td>
                            <td>Static</td>
                        </tr>
                         <tr class="info">
                            <td>192.168.0.3</td>
                            <td>Device #2</td>
                            <td>EE:EE:EE:EE</td>
                            <td>DHCP</td>
                        </tr>

                    </tbody>
                </table>


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
                    <tbody>
                        <tr class="success">
                            <td>1</td>
                            <td>Hard Disk Backup</td>
                            <td><i class="icon-hdd"></i></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr class="warning">
                            <td>2</td>
                            <td>Generic Printer</td>
                            <td><i class="icon-print"></i></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr class="error">
                            <td>3</td>
                            <td>3G Modem</td>
                            <td><i class="icon-signal"></i></td>
                            <td></td>
                            <td></td>
                        </tr>
                    </tbody>
                </table>

        </div>
      </div><!--/row-->
  </div>
      <hr>

      <footer id="footer">
      </footer>

    </div><!--/.fluid-container-->

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
