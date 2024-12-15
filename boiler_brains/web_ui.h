//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//------------------------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------------------------
String AjaxRefreshJS(String AjaxID,String Query,String RefreshMS) { // Refreshes data in a card on a random timed basis
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
String CreateLink(String LinkText,String FormTitle,String FormID) { // Creates a link for editing a setting
  return "<a href=\"javascript:void(0);\" onClick=\"LoadForm('" + FormTitle + "','" + FormID + "')\"><span class=\"text-info\">" + LinkText + "</span></a>";
}
//------------------------------------------------------------------------------------------------
String CreateModal() { // Bootstrap modal used as a popover container for forms
  String Content = "";
  Content += "<div class=\"modal fade\" id=\"dynamicModal\" tabindex=\"-1\" aria-labelledby=\"dynamicModalLabel\" aria-hidden=\"true\">";
  Content +=   "<div class=\"modal-dialog\">";
  Content +=     "<div class=\"modal-content\">";
  Content +=       "<div class=\"modal-header\">";
  Content +=         "<h5 class=\"modal-title\" id=\"dynamicModalLabel\">Form Title</h5>";
  Content +=         "<button type=\"button\" class=\"btn-close\" data-bs-dismiss=\"modal\" aria-label=\"Close\"></button>";
  Content +=       "</div>";
  Content +=       "<div id=\"modalContent\" class=\"modal-body\">";
  Content +=         "<div id=\"form-content\">Loading...</div>";
  Content +=       "</div>";
  Content +=       "<div class=\"modal-footer\" style=\"vertical-align: bottom;\">";
  Content +=         "<button type=\"button\" class=\"btn btn-sm btn-success\" id=\"submit_button\">&nbsp;Save&nbsp;</button>";
  Content +=       "</div>";
  Content +=     "</div>";
  Content +=   "</div>";
  Content += "</div>\n\n";

  return Content;
}
//------------------------------------------------------------------------------------------------
String DrawCard(String Body,String AjaxID,String Query,bool DoAjax) { // Plots a Bootstrap card
  String Content = "";
  if (DoAjax) Content += AjaxRefreshJS(AjaxID,Query,"4000");
  Content += "<div class=\"card\" style=\"width: 100%;margin-top: 0.5em;margin-bottom: 0.5em;margin-left: 0.5em;margin-right: 0.5em;\">";
  Content +=   "<div class=\"card-body\">";
  Content +=     "<div id=\"" + AjaxID + "\">";
  Content +=      Body;
  Content +=     "</div>";
  Content +=   "</div>";
  Content += "</div>";

  return Content;
}
//------------------------------------------------------------------------------------------------
String formatMillis(unsigned long millisValue) { // Converts a timestamp to a HH:MM:SS formated time
  unsigned long seconds = millisValue / 1000; // Convert milliseconds to seconds
  unsigned long minutes = seconds / 60;       // Convert seconds to minutes
  unsigned long hours = minutes / 60;         // Convert minutes to hours

  // Calculate the remaining seconds, minutes after accounting for hours and minutes
  seconds %= 60;
  minutes %= 60;

  char buffer[10]; // Enough space for "HH:MM:SS" + null terminator
  snprintf(buffer,sizeof(buffer),"%02ld:%02ld:%02ld",hours,minutes,seconds);

  return String(buffer);
}
//------------------------------------------------------------------------------------------------
String get_Form(byte WhichOne) { // Dynamically creates the form for the specified setting
  String Content = "";
  String Label,Value,Step,Min,Max;

  if (WhichOne == 0) {
    Label = "0 = Constant Power, 1 = Constant Temp";
    Step = "1"; Min = "0"; Max = "1"; Value = String(OpMode);
  } else if (WhichOne == 1) {
    Label = "0.0 to 260.0 (C)";
    Step = ".1"; Min = "0"; Max = "260"; Value = String(TargetTemp,1);
  } else if (WhichOne == 2) {
    Label = "10% to 100%";
    Step = "1"; Min = "10"; Max = "100"; Value = String(StartupPercent);
  } else if (WhichOne == 3) {
    Label = "10% to 100%";
    Step = "1"; Min = "10"; Max = "100"; Value = String(FallBackPercent);
  } else if (WhichOne == 4) {
    Label = "1% to 100%";
    Step = "1"; Min = "1"; Max = "100"; Value = String(AdjustRate);
  } else if (WhichOne == 5) {
    Label = "0.1 to 5.0 (C)";
    Step = ".1"; Min = ".1"; Max = "5"; Value = String(Deviation,1);
  } else if (WhichOne == 6) {
    Label = "1 to 1000";
    Step = "1"; Min = "1"; Max = "1000"; Value = String(ChangeWait);
  } else if (WhichOne == 7) {
    Label = "1 to 1000";
    Step = "1"; Min = "1"; Max = "1000"; Value = String(RestPeriod);
  }

  Content += "<form id=\"modalForm\">";
  Content +=   "<label for=\"data_" + String(WhichOne) + "\" class=\"form-label\">" + Label + "</label>";
  Content +=   "<input type=\"number\" step=\"" + Step + "\" min=\"" + Min + "\" max=\"" + Max + "\" class=\"form-control\" id=\"data_" + String(WhichOne) + "\" name=\"data_" + String(WhichOne) + "\" value=\"" + Value + "\">";
  Content += "</form>";

  return Content;
}
//------------------------------------------------------------------------------------------------
String InfoLine(String Title,String Data) { // Formats a line of text in a card
  return "<div class=\"row\"><div class=\"col-5\" style=\"padding: 2px;\"><p class=\"fw-bolder text-success mb-0\">" + Title + ":</p></div><div class=\"col-7\" style=\"padding: 2px;\"><p class=\"fw-bolder mb-0\" style=\"text-align: right;\">" + Data + "</p></div></div>";
}
//------------------------------------------------------------------------------------------------
// Homepage generation functions
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
  Content +=   "<link rel=\"icon\" href=\"https://panhandleponics.com/wp-content/uploads/2024/12/boilermaker.png?v=1.1\">";
  Content +=   "\n<style type=\"text/css\">\n";
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
  Content +=   "  a,a:hover {text-decoration: none;}\n";
  Content +=   "</style>\n";
  Content += "</head>";
  Content += "<body data-theme=\"dark\">";

  return Content;
}
//------------------------------------------------------------------------------------------------
String PageFooter() {
  String Content = "";
  Content += "<div id=\"hiddenDiv\" style=\"display: none;\"></div>";
  Content += "\n<script type=\"text/javascript\">\n";
  Content += "function ToggleRun() {\n";
  Content += "  if (confirm('Are you sure that you change the current run state?')) {\n";
  Content += "    jQuery('#hiddenDiv').load('./toggle-run');\n";
  Content += "  };\n";
  Content += "};\n\n";

  Content += "function LoadForm(FormTitle,ID) {\n";
  Content += "  jQuery('#form-content').load('./form-' + ID,function(response,status,xhr) {\n";
  Content += "    if (status === 'success') {\n";
  Content += "      jQuery('#dynamicModalLabel').html(FormTitle);\n";
  Content += "    } else {\n";
  Content += "      jQuery('#form-content').html('Failed to load `' + FormTitle + '` form content');\n";
  Content += "    }\n";
  Content += "    jQuery('#dynamicModal').modal('show');\n";
  Content += "    jQuery('.modal-backdrop').css('opacity','0.4');\n";
  Content += "  });\n";
  Content += "};\n";
  Content += "</script>\n";
  Content +=   "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/js/bootstrap.min.js\" integrity=\"sha384-mQ93GR66B00ZXjt0YO5KlohRA5SY2XofN4zfuZxLkoj1gXtW8ANNCe9d5Y3eG5eD\" crossorigin=\"anonymous\"></script>";
  Content += "</body>";
  Content += "</html>";

  return Content;
}
//------------------------------------------------------------------------------------------------
String StaticData() {
  String Content = "";
  Content += InfoLine("Name",DeviceName);
  Content += InfoLine("Slave&nbsp;Units",String(SlaveTotal()));
  Content += "<button onClick=\"ToggleRun()\" class=\"btn btn-sm btn-outline-success\" type=\"button\" style=\"width: 100%; margin-top: .75em; margin-bottom: .5em;\">Start / Stop</button>";

  return Content;
}
//------------------------------------------------------------------------------------------------
String LiveData() {
  String Content = "";
  String Temp = "";
  if (ActiveRun) {
    Temp = "<span class=\"text-danger blink\">Active</span>";
  } else {
    Temp = "<span class=\"text-warning\">Inactive</span>";
  }
  Content += InfoLine("Run&nbsp;State",Temp);
  Content += InfoLine("Uptime",Uptime);
  Content += InfoLine("Runtime",Runtime);
  Temp = String(TempC,1) + "C / " + String(TempF,1) + "F";
  Content += InfoLine("Temperature",Temp);
  Content += InfoLine("Power&nbsp;Level",String(round(0.392156863 * PowerLevel),0) + "%");

  return Content;
}
//------------------------------------------------------------------------------------------------
String SettingsData() {
  String Content = "";
  String Temp = "";
  if (OpMode == 0) {
    Temp = "Constant Power";
  } else {
    Temp = "Constant Temp";
  }
  if (! ActiveRun) {
    Content += InfoLine("Current&nbsp;Mode",CreateLink(Temp,"Operation Mode","0"));
  } else {
    Content += InfoLine("Current&nbsp;Mode",Temp);
  }
  Temp = String(TargetTemp,1) + "C / " + String(TargetTemp * 9 / 5 + 32,1) + "F";
  Content += InfoLine("Target&nbsp;Temp",CreateLink(Temp,"Target Temperature","1"));
  Content += InfoLine("Startup&nbsp;Power",CreateLink(String(StartupPercent) + "%","Startup Power Level","2"));
  Content += InfoLine("Fallback&nbsp;Power",CreateLink(String(FallBackPercent) + "%","Fallback Power Level","3"));
  Content += InfoLine("Adjustment&nbsp;Rate",CreateLink(String(AdjustRate) + "%","Adjustment Power Rate","4"));
  Temp = String(Deviation,1) + "C / " + String(Deviation * 9 / 5,1) + "F";
  Content += InfoLine("Deviation&nbsp;Rate",CreateLink("&plusmn; " + Temp,"Deviation Rate (C)","5"));
  Content += InfoLine("Change&nbsp;Wait",CreateLink(String(ChangeWait) + " secs","Change Wait Time (seconds)","6"));
  Content += InfoLine("Rest&nbsp;Period",CreateLink(String(RestPeriod) + " secs","Rest Period (seconds)","7"));

  return Content;
}
//------------------------------------------------------------------------------------------------
String HomePage() {
  String Content = "";
  Content += PageHeader();
  Content += CreateModal();
  Content += "<div class=\"container-fluid\" style=\"align: left;\">";
  Content +=   "<div class=\"row\">";
  Content +=   "<div style=\"display: flex; align-items: center;\"><span class=\"iconify\" style=\"font-size: 3em;\" data-icon=\"token-branded:dzoo\"></span>&nbsp;<span class=\"fw-bolder\" style=\"font-size: 1.5em;\">Boilermaker&nbsp;v" + Version + "</span></col>";
  Content +=   "</div>";
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
