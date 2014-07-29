<?php
error_reporting(E_ALL&~E_NOTICE);
function runPHP() {
	if ($_POST['cmd'] != 'checkhex') return;
	if ($_FILES['uphex']['error']==0 && is_uploaded_file($_FILES['uphex']['tmp_name']) && $_FILES['uphex']['size']) {
		$buf = file_get_contents($_FILES['uphex']['tmp_name']);
		if (preg_match('/[^:\dA-F\r\n]/is', $buf)) {
			echo '<font color="red">File format error!</font>';
			return;
		}
		$lines = preg_split('/(\r\n)|[\r\n]/is', $buf);
		$data_blocks = array();
		$block_name = '0000';
		$begin_addr = '';
		for ($i = 0; $i < count($lines); $i++) {
			$line = $lines[$i];
			if (preg_match('/^:(\w{2})(\w{4})(\w{2})(\w*)(\w{2})$/is', $line, $m)) {
				$data_len = hexdec($m[1]);
				$addr = hexdec($m[2]);
				$type = hexdec($m[3]);
				$chk = hexdec($m[5]);
				if (strlen($m[4]) != $data_len * 2) {
					echo '<font color="red">Line #'.$i.' data error!</font><br>';
					continue;
				}
				$sum = 0;
				for ($j = 1; $j < strlen($line) - 2; $j += 2) {
					$sum += hexdec($line{$j}.$line{$j+1});
					$sum &= 0xFF;
				}
				$sum = (~$sum + 1) & 0xFF;
				if ($sum != $chk) {
					echo '<font color="red">Line #'.$i.' checksum error!</font>'.sprintf('%02X(%02X)', $sum, $chk).'<br>';
					exit;
				}
				if ($type == 1) {
					break;
				}
				$datas = array();
				for ($j = 0; $j < $data_len; $j++) {
					$datas[$j] = hexdec($m[4]{$j*2}.$m[4]{$j*2+1});
				}
				switch($type) {
				case 0:
					for ($j = 0; $j < $data_len; $j++) {
						$data_blocks[$block_name][$addr + $j] = $datas[$j];
					}
					break;
				case 2:
					$hi_addr = ($datas[0] << 8) | $datas[1];
					$block_name = sprintf('%04X', $hi_addr);
					$data_blocks[$block_name] = array();
					break;
				case 4:
					$hi_addr = ($datas[0] << 8) | $datas[1];
					$block_name = sprintf('%04X', $hi_addr);
					$data_blocks[$block_name] = array();
					break;
				case 5:
					$begin_addr = substr($line, 9, $data_len * 2);
					break;
				default:
					echo '<font color="red">Line #'.$i.' type error!</font><br>';
					continue;
				}
			}
		}
		foreach ($data_blocks as $block_name => $block_datas) {
			echo '<div style="border:1px solid gray; font-size:9pt; font-family:Fixedsys">';
			for ($i = 0; $i < count($block_datas); $i++) {
				if (($i & 0x0F) == 0) echo '<br>'.$block_name.sprintf('%04X', $i).':';
				printf(' %02X', $block_datas[$i]);
			}
			echo '</div>';
		}
		if ($begin_addr) {
			echo 'Start Addr: <font color:"blue">'.$begin_addr.'</font><br>';
		}
	}
}
runPHP();
?>
<form enctype="multipart/form-data" method="POST" target="_blank">
	<input type="hidden" name="cmd" value="checkhex">
	<input type="hidden" name="MAX_FILE_SIZE" value="2097152">
	Upload Hex File: <input type="file" name="uphex"><br>
	<input type="submit" value="Submit">
</form>
