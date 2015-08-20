<?php
$link = mysql_connect('localhost','root','shiva');
if (!$link) { die('Could not connect: ' . mysql_error()); }
mysql_select_db('workshop_info');

$result = mysql_query("SELECT id, foss_category, cfm_wkshop_date, cfm_wkshop_time FROM workshop_requests where wr.status = 2");

// Initializes a container array for all of the calendar events
$jsonArray = array();

while($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
 $eventtest = $row['foss_category'];
 $eventdate = $row['cfm_wkshop_date'];
 $eventdate_start = $row['cfm_wkshop_date']." ".$row['cfm_wkshop_time'];
 $incubation = "02:00:00";
 $temp = date( "H:i:s", strtotime($row['cfm_wkshop_time']) + 2 * 3600 );
 $eventdate_end = $row['cfm_wkshop_date']." ".$temp;
 $eventid = $row['id'];
 $eventurl = "http://10.118.228.46/spoken_tutorial_org_old/events/wkshp_detail/".$eventid;
 // Stores each database record to an array
 $buildjson = array('title' => "$eventtest", 'start' => "$eventdate_start",'end' => "$eventdate_end", 'url' => "$eventurl",'allDay' => false);

 // Adds each array into the container array
 array_push($jsonArray, $buildjson);

}
// Output the json formatted data so that the jQuery call can read it
echo json_encode($jsonArray);
?>
