<?php
        $con = mysqli_connect("localhost","hahat17","10836gkwngus!","db");

        $card_id = $_GET['card_id'];

        $sub_sql = "select * from membership where card_id = '$card_id'";
        $sub_result = mysqli_query($con, $sub_sql);
        $sub_row = mysqli_fetch_array($sub_result);

        $nostock_sql = "select * from borrow" ;
        $nostock_result = mysqli_query($con, $nostock_sql);
        if(mysqli_num_rows($sub_result) == 0){
                echo "0";
                mysqli_close($con);
	}
	else{

      		$check_sql = "select * from borrow where card_id = '$card_id'";
      		$check_result = mysqli_query($con, $check_sql);


      		if(mysqli_num_rows($check_result) > 0){
			$sql = "delete from borrow where card_id='$card_id'";
			mysqli_query($con, $sql);
	
			$sql = "update membership set return_check = 1, return_time = CURRENT_TIMESTAMP where card_id = '$card_id' ";
                	mysqli_query($con, $sql);

                	mysqli_close($con);
			echo "2";
	
      		}
		else if(mysqli_num_rows($nostock_result) == 3){
			echo "3";
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
			echo "1";
		}	
	
	}
     // echo ("<meta http-equiv='Refresh' content='1; URL=h_index_test.html'>");

?>
