<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Logs - DSL-N55U</title>
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
          <div class="hero-unit">
            <h1>DSL-N55U Logs</h1>
            <p>Here you can find different verbose logs of your device.</p>
            </div>
            <div class="row-fluid">

                <h2>DSL Log</h2>
                <textarea><% nvram_dump("adsl/adsllog.log",""); %></textarea>
                <hr />

                <h2>General Log</h2>
                <textarea><% nvram_dump("syslog.log","syslog.sh"); %></textarea>
                <hr />

                <h2>DHCP Leases</h2>
                <textarea><% nvram_dump("leases.log", "leases.sh"); %></textarea>
                <hr />

                <h2>Wireless Log</h2>
                <textarea><% nvram_dump("wlan11b_2g.log","wlan11b.sh"); %></textarea>
                <hr />

                <h2>Port Forwarding Log</h2>
                <textarea><% nvram_dump("iptable.log","iptable.sh"); %></textarea>
                <hr />

                <h2>Routing Table</h2>
                <textarea><% nvram_dump("route.log","route.sh"); %></textarea>
                <hr />

                <h2>Connections</h2>
                <textarea><% nvram_dump("syscmd.log","../usr/sbin/netstat-nat.sh > /tmp/syscmd.log 2>&1"); %></textarea>
                <hr />

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
    <script>
        $(document).ready(function(){
                // dynamic layout
                showSidebar();
                showFooter();
        });
    </script>
  </body>
</html>
