<?php
	$con=mysqli_connect("localhost","user","passwd","db");
	if(mysqli_connect_errno())
	{
		echo "Faild to connect to MySQL:".mysqli_connect_error();
	}

         
	header("Content-Type:text/html;charset=utf8");   
	mysqli_query($con, "set session character_set_connection=utf8;");
	mysqli_query($con, "set session character_set_results=utf8;");
	mysqli_query($con, "set session character_set_client=utf8;");
	$card_id=$_GET['card_id'];
	$name=$_GET['name'];
	$phonenumber=$_GET['phonenumber'];
	$address=$_GET['address'];
	header("Content-Type:text/html;charset=utf8");
	$sql="insert into db.membership(card_id,name,phonenumber,address) values('$card_id','$name','$phonenumber','$address')";
	mysqli_query($con,$sql);
	mysqli_close($con);
	echo("<meta http-equiv='Refresh' content='1; URL=index.html'>");
?>
