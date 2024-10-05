<?php
if (isset($_FILES['file'])) {
    $file_name = $_FILES['file']['name'];
    $tmp_name = $_FILES['file']['tmp_name'];
    $file_up_name = time() . "_" . $file_name; 

    if (!is_dir('files')) {
        mkdir('files', 0777, true);
    }

    if (move_uploaded_file($tmp_name, "files/" . $file_up_name)) {
        echo "File uploaded successfully!";
    } else {
        echo "Failed to upload file!";
    }
} else {
    echo "No file was uploaded!";
}
?>
