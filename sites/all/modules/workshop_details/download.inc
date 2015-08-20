<?php
    function download()
    {
        require_once('/Sites/spoken_tutorial_org/sites/all/modules/workshop_details/fpdf/fpdf.php');
        require_once('/Sites/spoken_tutorial_org/sites/all/modules/workshop_details/FPDI/fpdi.php');

        $workshop_code = arg(2);
        $id = arg(3);
        $query = "select wr.foss_category, ac.institution_name, o.organiser_name, wr.cfm_wkshop_date from workshop_info.workshop_requests wr, workshop_info.academic_center ac, workshop_info.organiser o where wr.workshop_code = '{$workshop_code}' and wr.academic_code = ac.academic_code and wr.organiser_id = o.organiser_id";
		$result = db_query($query);		
		$data = db_fetch_object($result);

		$query_name = "select * from workshop_info.student_detail where id=".$id;
		$result_name = db_query($query_name);
		$data_name = db_fetch_object($result_name);
		global $user;
		$pdf = new FPDI('L', 'mm', 'Letter');
		$pageCount = $pdf->setSourceFile("/Sites/spoken_tutorial_org/sites/all/modules/workshop_details/Blank.pdf");
		$tplIdx = $pdf->importPage(1, '/MediaBox');
		$pdf->addPage();
		$pdf->useTemplate($tplIdx, 10, 10, 265,200);
		if (!$pdf)
		{
			echo "Error!";
		}
		$pdf->SetMargins(18,1,18);
		$pdf->SetFont('Arial','B',20);
		$pdf->Ln(38);
		$pdf->Cell(240,8,'CERTIFICATE OF LEARNING', 0,1,'C');
		$pdf->Ln(5);
		$pdf->SetFont('Arial','B',12);
		$pdf->Ln(19);
		$pdf->SetFont('Arial','',12);
		$pdf->SetX(40);
        if($data_name->last_name != ' ')
        {
		$pdf->MultiCell(210,8,'This is to certify that '.$data_name->first_name.' '.$data_name->last_name.' participated in the '.$data->foss_category.' workshop organized at '.$data->institution_name.' by '.$data->organiser_name.' on '.$data->cfm_wkshop_date.' with course material provided by the Talk To A Teacher project at IIT Bombay.', 0);
        }
        else
        {
        $pdf->MultiCell(210,8,'This is to certify that '.$data_name->first_name.' participated in the '.$data->foss_category.' workshop organized at '.$data->institution_name.' by '.$data->organiser_name.' on '.$data->cfm_wkshop_date.' with course material provided by the Talk To A Teacher project at IIT Bombay.', 0);
        }
		$pdf->Ln(5);
		$pdf->SetX(40);
		$pdf->SetFont('Arial','',12);
		$pdf->SetTextColor(0,0,0);
		$pdf->MultiCell(210,8,'A comprehensive set of topics pertaining to '.$data->foss_category.' were covered in the workshop. This workshop is offered by the Spoken Tutorial project, IIT Bombay, funded by National Mission on Education through ICT, MHRD, Govt., of India.', 0);
		$pdf->SetX(170);
		$pdf->SetFont('','U');
		$pdf->SetTextColor(0,0,255);
		$pdf->SetX(198);
		$pdf->SetFont('','');
		$pdf->SetTextColor(0,0,0);
		$pdf->SetY(-50);
		$pdf->SetX(190);
		$pdf->SetTextColor(0,0,0);
		$pdf->SetFont('','B');
        $pdf->Image($path."/Sites/spoken_tutorial_org/sites/all/modules/workshop_details/sign.jpg", 204, 151, 0, 23);
        //$pdf->SetX(206);
		//$pdf->Cell(0,7,'Prof. Kannan M Moudgalya', 0,1,'L');
		$pdf->SetX(195);
		$pdf->SetX(195);
		$pdf->SetX(216);
		//$pdf->Cell(0,7,'IIT Bombay', 0,1,'L');
		$pdf->SetY(160);
		$pdf->SetFont('','');
		$pdf->SetX(39);
		$cur_date=date('jS F, Y');
		$pdf->Cell(200,0,' Date:  '.$cur_date.'',0,1,'L');
		$pdf->SetX(40);
		$pdf->Cell(200,15,'Email: contact@spoken-tutorial.org', 0,1,'L');
		$pdf->Output();
}
?>

