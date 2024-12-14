//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//------------------------------------------------------------------------------------------------
String formatMillis(unsigned long millisValue) {
  unsigned long seconds = millisValue / 1000; // Convert milliseconds to seconds
  unsigned long minutes = seconds / 60;       // Convert seconds to minutes
  unsigned long hours = minutes / 60;         // Convert minutes to hours

  // Calculate the remaining seconds, minutes after accounting for hours and minutes
  seconds %= 60;
  minutes %= 60;

  char buffer[10]; // Enough space for "HH:MM:SS" + null terminator
  snprintf(buffer, sizeof(buffer), "%02ld:%02ld:%02ld", hours, minutes, seconds);

  return String(buffer);
}
//------------------------------------------------------------------------------------------------
String PageHeader() {
  String Content = "";
  Content += "<!DOCTYPE html>";
  Content += "<html lang=\"en\">";
  Content += "<head>";
  Content +=   "<title>" + DeviceName + "</title>";
  Content +=   "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,shrink-to-fit=no\">";
  Content +=   "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-GLhlTQ8iRABdZLl6O3oVMWSktQOp6b7In1Zl3/Jr59b6EGGoI1aFkw7cmDA6j6gD\" crossorigin=\"anonymous\">";
  Content +=   "<link rel=\"stylesheet\" href=\"https://unpkg.com/bootstrap-darkmode@0.7.0/dist/darktheme.css\">";
  Content +=   "<script src=\"https://code.iconify.design/2/2.0.3/iconify.min.js\"></script>";
  Content +=   "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.6.4/jquery.min.js\"></script>";
  Content +=   "<link rel=\"icon\" href=\"https://panhandleponics.com/favicon.ico?v=1.1\">";
  Content +=   "\n<style type="text/css">\n";
  Content +=   "  @-webkit-keyframes blinker {\n";
  Content +=   "    from {opacity: 1.0;}\n";
  Content +=   "    to {opacity: 0.0;}\n";
  Content +=   "  }\n";
  Content +=   "  .blink {\n";
  Content +=   "    text-decoration: blink;\n";
  Content +=   "    -webkit-animation-name: blinker;\n";
  Content +=   "    -webkit-animation-duration: 0.6s;\n";
  Content +=   "    -webkit-animation-iteration-count:infinite;\n";
  Content +=   "    -webkit-animation-timing-function:ease-in-out;\n";
  Content +=   "    -webkit-animation-direction: alternate;\n";
  Content +=   "  }\n";
  Content +=   "</style>\n";
  Content += "</head>";
  Content += "<body data-theme=\"dark\">";

  return Content;
}
//------------------------------------------------------------------------------------------------
String PageFooter() {
  String Content = "";
  //Content += "\n<script type=\"text/javascript\">\n";
  //Content += "</script>\n";
  Content +=   "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/js/bootstrap.min.js\" integrity=\"sha384-mQ93GR66B00ZXjt0YO5KlohRA5SY2XofN4zfuZxLkoj1gXtW8ANNCe9d5Y3eG5eD\" crossorigin=\"anonymous\"></script>";
  Content += "</body>";
  Content += "</html>";

  return Content;
}
//------------------------------------------------------------------------------------------------
String AjaxRefreshJS(String AjaxID,String Query,String RefreshMS) {
  String Content = "";
  Content += "\n<script type=\"text/javascript\">\n";
  Content += "  jQuery(document).ready(function() {\n";
  Content += "    RandomDelay = " + RefreshMS + " + Math.floor(Math.random() * 1000) + 1;\n";
  Content += "    function refresh() {\n";
  Content += "      jQuery('#" + AjaxID + "').load('./" + Query + "');\n";
  Content += "    }\n";
  Content += "    setInterval(function() {\n";
  Content += "      refresh()\n";
  Content += "    },RandomDelay);\n";
  Content += "  });\n";
  Content += "</script>\n";

  return Content;
}
//------------------------------------------------------------------------------------------------
String InfoLine(String Title,String Data) {
  return "<span class=\"fw-bolder text-success\">" + Title + ":&nbsp;&nbsp;</span><span class=\"fw-bolder\">" + Data + "</span>";
}
//------------------------------------------------------------------------------------------------
String DrawCard(String Body,String AjaxID,String Query,bool DoAjax) {
  String Content = "";
  if (DoAjax) Content += AjaxRefreshJS(AjaxID,Query,"4000");
  Content += "<div class=\"card\" style=\"width: 95%;margin-top: 0.5em;margin-bottom: 0.5em;margin-left: 0.5em;margin-right: 0.5em;\">";
  Content +=   "<div class=\"card-body\">";
  Content +=     "<div id=\"" + AjaxID + "\">";
  Content +=      Body;
  Content +=     "</div>";
  Content +=   "</div>";
  Content += "</div>";

  return Content;
}
//------------------------------------------------------------------------------------------------
String StaticData() {
  String Content = "";
  Content += InfoLine("Name",DeviceName) + "<br>";
  Content += InfoLine("Slave&nbsp;Units",String(SlaveTotal()));

  return Content;
}
//------------------------------------------------------------------------------------------------
String LiveData() {
  String Content = "";
  Content += InfoLine("Uptime",Uptime) + "<br>";
  Content += InfoLine("Runtime",Runtime);

  return Content;
}
//------------------------------------------------------------------------------------------------
String SettingsData() {
  String Content = "";

  return Content;
}
//------------------------------------------------------------------------------------------------
String HomePage() {
  String Content = "";
  Content += PageHeader();
  Content += "<div class=\"container-fluid\" style=\"align: left;\">";
  Content +=   "<div class=\"row\">";
  Content +=    DrawCard(StaticData(),"TopCard","",false);
  Content +=   "</div>";
  Content +=   "<div class=\"row\">";
  Content +=    DrawCard(LiveData(),"LiveData","ajax-livedata",true);
  Content +=   "</div>";
  Content +=   "<div class=\"row\">";
  Content +=    DrawCard(SettingsData(),"SettingsData","ajax-settings",true);
  Content +=   "</div>";
  Content += "</div>";
  Content += PageFooter();

  return Content;
}
//------------------------------------------------------------------------------------------------
