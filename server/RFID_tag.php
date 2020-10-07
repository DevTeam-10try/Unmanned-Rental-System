<?php
	$con = mysqli_connect("localhost","username","password","db");

	$card_id = $_GET['card_id'];

      	$sub_sql = "select * from membership where card_id = '$card_id'";
      	$sub_result = mysqli_query($con, $sub_sql);
      	$sub_row = mysqli_fetch_array($sub_result);

      	$check_sql = "select * from borrow where card_id = '$card_id'";
      	$check_result = mysqli_query($con, $check_sql);

      	if(mysqli_num_rows($check_result) > 0){
		$sql = "delete from borrow where card_id='$card_id'";
		mysqli_query($con, $sql);
	
		$sql = "update membership set return_check = 1, return_time = CURRENT_TIMESTAMP where card_id = '$card_id' ";
                mysqli_query($con, $sql);

                mysqli_close($con);
	
      	}	
      	else{
		$name = $sub_row['name'];
		$phone = $sub_row['phonenumber'];
      		$sql = "insert into borrow(card_id, name, phonenumber) values('$card_id', '$name', '$phone') ";
      		mysqli_query($con, $sql);

		$sql = "update membership set return_check = 0, borrow_time = CURRENT_TIMESTAMP where card_id = '$card_id' ";
		mysqli_query($con, $sql);

      		mysqli_close($con);
	}
?>