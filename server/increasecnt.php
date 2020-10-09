<?php
		parse_str(getenv("QUERY_STRING"));
    		$conn = mysqli_connect("localhost","hahat17","10836gkwngus!", "db") or die("fail");
    		//db 연결
    		$dbconn = mysqli_select_db($conn, $mysql_db);
    		//charset UTF8
    		mysqli_query("set names utf 8");
    		//쿼리문 작성
    		$query = "select alert from membership where card_id='".$card_id."'";
    		
      		//쿼리보내고 결과를 변수에 저장
    		$result = mysqli_query($conn, $query);
		$row = mysqli_fetch_array($result);
		$input = $row[alert]+1 ;
		$query2 = "update membership set alert=".$input." where card_id='".$card_id."'";
		
		$result2 = mysqli_query($conn, $query2);
		mysqli_close($conn);

		echo("<meta http-equiv='Refresh' content='1; URL=index.html'>");

    	?>
