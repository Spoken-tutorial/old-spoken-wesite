<?php
	require_once('common.inc');
	function resource_center_map_details($code){
		
		$path = drupal_get_path('module', 'workshops');
		drupal_add_css($path . '/rc_star_rating.css', 'module');
		$output = '';
		$states_array = get_map_states(0);
		
		if(isset($code) && $code != '') {
			$output .= '<p>State : <b>'.$states_array[$code].'</b></p>';
			$query = "select id, academic_code, institution_name, city, contact_details, remarks, star_rating from academic_center where state_code='".$code."' and resource_center=1";
			$sql_count = "select count(id) from academic_center where state_code='".$code."' and resource_center=1";
		
			$header = array(
				array('data'=>'Institution', 'field'=>'institution_name', 'sort_order'=>'asc'),
				array('data'=>'City', 'field'=>'city'),
				array('data'=>'Rating', 'field'=>'star_rating'),
				array('data'=>'Action')
			);
			$query .= tablesort_sql($header);
			db_set_active('workshop_info');
			$result = pager_query($query, 30, 0, $sql_count);
			db_set_active('default');
			$row = array();
			$count = 0;
		
			while ($item = db_fetch_object($result)) {
				$stars = '';
				$star_count = (int)($item->star_rating);
				$path = "star2.gif";
				for($i = 0; $i < $star_count; $i++) {
					$stars .= '<div class="rc-star-rating"></div>';
				}
				$row[] = array($item->institution_name, $item->city, $stars, l(t('View Activities'), 'resource_center_view_college_map_details/'. $item->id));
				$count++;
			}
			if($count == 0) {
				$output .= "<div class='academic-data'>";
					$output .= "<p>There is no Resource Center for this State</p>";
				$output .= "</div>";
			}else{
				$output .= theme_table($header,$row);
				$output .= theme_pager(null, 30);
			}
		}else {
			$output .= "<div class='academic-data'>";
				$output .= "<p>Something went wrong!</p>";
			$output .= "</div>";
		}

		return $output;
	}
	
	function resource_center_view_college_map_details($id = NULL){
		if($id){
			$states_array = get_map_states(0);
	 		$query = "select * from {academic_center} where id=".$id;
	 		$path = drupal_get_path('module', 'workshops');
			drupal_add_css($path . '/rc_star_rating.css', 'module');
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
			$states_array = get_map_states(0);
	 		if($result){
	 			$item = db_fetch_object($result);
	 			$stars = '';
				$star_count = (int)($item->star_rating);
				$path = "star2.gif";
				for($i = 0; $i < $star_count; $i++) {
					$stars .= '<div class="rc-star-rating"></div>';
				}
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>State</b></td><td>'.$states_array[$item->state_code].'</td></tr>';
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td>'.$item->academic_code.'</td></tr>';
	 			$tmp = ($item->school_college)? "College" : "School";
	 			$output .= '<tr><td><b>College / School</b></td><td>'.$tmp.'</td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td>'.$item->institution_name.'</td></tr>';
	 			$output .= '<tr><td><b>Address</b></td><td>'.$item->street.'</td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td>'.$item->city.'</td></tr>';
	 			$output .= '<tr><td><b>State</b></td><td>'.$states_array[$item->state_code].'</td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td>'.$item->pincode.'</td></tr>';
	 			$output .= '<tr><td><b>Rating</b></td><td>'.$stars.'</td></tr>';
	 			$output .= '<tr><td><b>Contact Details</b></td><td><p>'.str_replace("\n", "<br />", $item->contact_details).'</p></td></tr>';
	 			$output .= '<tr><td><b>Activities</b></td><td><p>'.str_replace("\n", "<br />", activities_human_view($item->remarks)).'</p></td></tr>';
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';
	 			$output .= '</table>';

	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
	 	}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}

function activities_human_view($outline){
        $temp = str_replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;", $outline);
        $temp = str_replace("\n\n\n", "\n", $temp);
        $outline_array = explode("\n", $temp);
        $temp = "";
        foreach($outline_array as $line) {
                $line = trim($line);
                if($line != ''){
                        $temp .= "<li>".$line."</li>";
                }
        }
        return "<ul>".$temp."</ul>";
}
