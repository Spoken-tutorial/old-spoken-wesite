<?php

function missing_tutorial_all() {

	global $user;
	global $base_path;
    drupal_add_css(drupal_get_path("module", "missing_component") . "/css/missing_component.css");
    drupal_add_js(drupal_get_path("module", "missing_component") . "/js/missing_cmponent.js");
    $page_content = "";

    $query = "select tmc.id, tmc.uid, tmc.trid, tmc.component, tmc.type, tmc.remarks, tmc.reported, tmc.reply_status, tmc.created, tmc.email, tr.language, tr.tutorial_outline_uid, tr.tutorial_script_uid, tr.tutorial_video_uid, tcc.tutorial_slide_uid, tcc.tutorial_code_uid, tcc.tutorial_assignment_uid, td.tutorial_name, td.foss_category from tutorial_missing_component as tmc,tutorial_resources as tr, tutorial_common_contents as tcc, tutorial_details as td where tr.id=tmc.trid and tcc.id=tr.tutorial_content_id and td.id=tr.tutorial_detail_id ORDER BY tmc.created DESC";

    $count = "select count(*) from tutorial_missing_component as tmc,tutorial_resources as tr, tutorial_common_contents as tcc, tutorial_details as td where tr.id=tmc.trid and tcc.id=tr.tutorial_content_id and td.id=tr.tutorial_detail_id";

    $query_result = pager_query($query, 5 , 0, $count);

	while ($item = db_fetch_object($query_result)) {

		$posted_user = user_load($item->uid);
        $border = $row->reply_status?"sent":"not-sent";

        $page_content .= t("<div class='feedback {$border}'>");
        $page_content .= t("<div class='timestamp'>") . date('d-M-Y',strtotime($item->created)) . t("</div>");

        if($item->uid && $posted_user->mail) {
        	$page_content .= t("<div class='from left'> <b>From:</b> <span class='right'>") . $posted_user->mail . t("</span></div>");
        } elseif($item->email) {
	        $page_content .= t("<div class='from left'> <b>From:</b> <span class='right'>") . $item->email . t("</span></div>");
        }

        $page_content .= t("<div class='type left'> <b>FOSS Category:</b> <span class='right'>") . $item->foss_category . t("</span></div>");

        $page_content .= t("<div class='type left'> <b>Component:</b> <span class='right'>") . $item->component . t("</span></div>");

        $page_content .= t("<div class='type left'> <b>Language:</b> <span class='right'>") . $item->language . t("</span></div>");

        $page_content .= t("<div class='type left'> <b>Tutorial Name:</b> <span class='right'>") . $item->tutorial_name . t("</span></div>");

        $page_content .= t("<div class='type left'><b>Link To The Tutorial:</b><span class='right'>").t(
			"<a href='http://spoken-tutorial.org/show_video?tr=".$item->trid."&sval=1' target='_blank'>
				<strong>Click Here</strong>
			</a>"
		). t("</span></div>");

        $page_content .= t("<div class='type reply'><b>Remarks:</b></div><p style='text-align: justify;'>") . $item->remarks . t("</p>");

		$query = "select uid, reply_message, created from tutorial_missing_component_reply where missing_component_id = ".$item->id." order by created ASC";
		$result = db_query($query);
		$flag = 1;
		while($row = db_fetch_object($result)) {
			if($flag) {
				$page_content .= t("<div class='reply'> <br><b>Reply Sent:</b></div>");
				$flag = 0;
			}
			$tmp_user = user_load($row->uid);
			$page_content .= "<p style='text-align: justify; clear: both; padding-top: 6px'>".$row->reply_message."</p>";
			$page_content .= "<div><div style='float: left;'>".time_elapsed_string(strtotime($row->created))."</div><div style='float: right; padding-bottom: 5px;'>- <a href='".$base_path."user/".$row->uid."' target='_blank'>".$tmp_user->name."</a></div></div><hr style='clear: both;' />";
		}
        if((in_array('contributor', array_values($user->roles)) && ($user->id == $item->uid)) || (in_array('admin_user', array_values($user->roles)))) {
        	if($item->reported) {
		        $page_content .= t("<span class='btn-reply'><a class='form_{$item->id}' href='#form_{$item->id}'>Reply</a></span><br>");
		        $page_content .= t("<div id='form_{$item->id}' class='form-wrapper'>");
		        $page_content .= drupal_get_form("missing_component_reply_form", $item->id);
		        $page_content .= t("</div>");
            }
        }
        $page_content .= t("</div>");
    } // while ends
	$page_content .= theme('pager', NULL, 5, 0);
	return $page_content;
} // function missing_tutorial_all

/*function missing_components_comments_forms($form_id) {
  $forms = array();
  if (strpos($form_id, 'missing_component_reply_form_') === 0) {
    $forms[$form_id] = array(
      'callback' => 'missing_component_reply_form',
    );
  }
  return $forms;
} // function cloud_comments_forms*/


function missing_component_reply_form($form_state, $comment_id) {

    $form = array();
    $form["#submit"] = array(
        'missing_component_reply_form_submit',
    );

    $form["content"] = array(
        '#type' => 'textarea',
        '#wysiwyg' => True,
        '#attributes' => array('rows' => '10', 'cols' => '64'),
    );
    $form["comment_id"] = array(
        '#type' => 'hidden',
        '#value' => $comment_id,
    );
    $form["submit"] = array(
        '#type' => 'submit',
        '#value' => "Send Mail"
    );

    return $form;
} // function reply_form

function missing_component_reply_form_submit($form, &$form_state) {
	global $user;

    $query = "insert into tutorial_missing_component_reply (missing_component_id, uid, reply_message) values(".$form_state["values"]["comment_id"].", ".$user->uid.", '".$form_state["values"]["content"]."')";

    if(db_query($query)) {
    	$query = "select uid, reported, email from tutorial_missing_component where id = ".$form_state["values"]["comment_id"];
    	$result = db_query($query);
    	$row = db_fetch_object($result);
    	$email = '';
    	if($row && $row->reported) {
    		if(!$row->email && $row->uid) {
    			$tmp_user = user_load($row->uid);
    			$email = $tmp_user->mail;
    		}elseif($row->email) {
				$email = $row->email;
			}
    	} 

    	if($email) {
    		$subject = 'Re: Component missing notification';
			$message = '<p>'.$form_state["values"]["content"].'</p>';
			$message .="<p>Thanks,</p><p>Administrator</p>";
			send_email($message, $subject, 'nancyvarkey@gmail.com, rahulkatarey@gmail.com, web-team@spoken-tutorial.org', '', $email, 'no-reply@spoken-tutorial.org');
			drupal_set_message('Reply message sent via email.');
    	} else {
    		drupal_set_message('To: email id is missing!');
    	}
    }
} // function reply_form_submit

function time_elapsed_string($ptime) {
	$etime = time() - $ptime;

	if ($etime < 1) {
		return '0 seconds ago';
	}

	$a = array( 12 * 30 * 24 * 60 * 60  =>  'year',
			30 * 24 * 60 * 60       =>  'month',
			24 * 60 * 60            =>  'day',
			60 * 60                 =>  'hour',
			60                      =>  'minute',
			1                       =>  'second'
		);

	foreach ($a as $secs => $str) {
		$d = $etime / $secs;
		if ($d >= 1) {
			$r = round($d);
			return $r . ' ' . $str . ($r > 1 ? 's' : '')." ago";
		}
	}
}

?>
