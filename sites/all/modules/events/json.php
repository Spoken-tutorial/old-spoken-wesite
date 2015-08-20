<?php
$link = mysql_connect('localhost','stadmin','Listdb*4321sT');
if (!$link) { die('Could not connect: ' . mysql_error()); }
mysql_select_db('workshop_info');

$result = mysql_query("SELECT DISTINCT (cfm_wkshop_date), count( cfm_wkshop_date ) as count,id, foss_category FROM workshop_requests where status = 1 GROUP BY cfm_wkshop_date");

// Initializes a container array for all of the calendar events
$jsonArray = array();

while($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
 $eventtest = $row['foss_category'];
 $eventdate = $row['cfm_wkshop_date'];
 $eventid = $row['id'];
 if($row['count']==1)
 { 
 	$count = $row['count']." workshop "; 
 }
 else
 { 
 	$count = $row['count']." workshops "; 
 }
 $eventurl = "http://www.spoken-tutorial.org/events/events_detail/".$eventdate;
 // Stores each database record to an array
 $buildjson = array('title' => "$count", 'start' => "$eventdate", 'url' => "$eventurl", 'tip' => "$eventtest");

 // Adds each array into the container array
 array_push($jsonArray, $buildjson);

}
// Output the json formatted data so that the jQuery call can read it
echo json_encode($jsonArray);
?>
