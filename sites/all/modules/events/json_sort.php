<?php
		$foss_arr = trim($_GET['name']);
		$state = trim($_GET['location']);
		$link = mysql_connect('localhost','stadmin','Listdb*4321sT');
		if (!$link) 
		{ 
			die('Could not connect: ' . mysql_error()); 
		}
		mysql_select_db('workshop_info');
		if($foss_arr && $state)
		{
			$result = mysql_query("SELECT wr.cfm_wkshop_date,wr.id, wr.foss_category,ac.city FROM workshop_info.workshop_requests as wr,workshop_info.organiser as o,workshop_info.academic_center as ac where wr.organiser_id = o.organiser_id and wr.academic_code = ac.academic_code and wr.foss_category LIKE '%".$foss_arr."%' and ac.state_code LIKE '%".$state."%' and wr.status = 1 GROUP BY cfm_wkshop_date");
		}
		else if(!$foss_arr && $state)
		{
			$result = mysql_query("SELECT wr.cfm_wkshop_date,wr.id, wr.foss_category,ac.city FROM workshop_info.workshop_requests as wr,workshop_info.organiser as o,workshop_info.academic_center as ac where wr.organiser_id = o.organiser_id and wr.academic_code = ac.academic_code and ac.state_code LIKE '%".$state."%' and wr.status = 1 GROUP BY cfm_wkshop_date");
		}
		else if($foss_arr && !$state)
		{
			$result = mysql_query("SELECT wr.cfm_wkshop_date,wr.id, wr.foss_category,ac.city FROM workshop_info.workshop_requests as wr,workshop_info.organiser as o,workshop_info.academic_center as ac where wr.organiser_id = o.organiser_id and wr.academic_code = ac.academic_code and wr.foss_category LIKE '%".$foss_arr."%' and wr.status = 1 GROUP BY cfm_wkshop_date");
		
		}
			
		// Initializes a container array for all of the calendar events
		$jsonArray = array();
		while($row = mysql_fetch_array($result, MYSQL_ASSOC))
		{	
			$eventcity = $row['city'];
			$eventfoss = $row['foss_category'];
			$eventid = $row['id'];
			$eventdate = $row['cfm_wkshop_date'];
			$eventurl = "http://www.spoken-tutorial.org/events/events_foss/".$eventid."/".$eventdate."/".$_GET['name']."/".$_GET['location'];
			// Stores each database record to an array
			$buildjson = array('title' => "$eventcity", 'start' => "$eventdate", 'url' => "$eventurl", 'tip' => "$eventcity");
			// Adds each array into the container array
			array_push($jsonArray, $buildjson);
		}
		
		// Output the json formatted data so that the jQuery call can read it
		echo json_encode($jsonArray);


?>
