var glBenchModule = null;  // Global application object.
var statusText = 'NO-STATUS';
var fileSystem = null;
var files = null;
var fileList = null;
var fileIndex = 0;
var fileCount = 0;
var filesLoaded = false;


// Indicate module load success.
function moduleDidLoad() {
  console.log("NaCl Module has loaded");
  glBenchModule = document.getElementById('glbench_nacl_vs10');
  updateStatus('NaCl Module has been loaded');
}

// Handle message sent from NaCL module ( using pp::Instance.PostMessage())
// Display to console
function handleMessage(message_event) {
  // Display to console
  console.log(message_event.data);
  // Parse incoming message string and update status with any messages prefixed by 'GLB'
  var newStr = new String(message_event.data);
  if (newStr.charAt(0) == 'G' && newStr.charAt(1) == 'L' && newStr.charAt(2) == 'B') {
    updateStatus(message_event.data);
  } else if (newStr == "ExitModule") {
    // ExitModule message ends NaCl module
    endModule();
  }
}

function endModule() {
  // Remove NaCl embed
  var element = document.getElementById("listener");
  document.body.removeChild(element);
  // Create download button
  var downloadButton = document.createElement('input');
  downloadButton.setAttribute('type', 'button');
  downloadButton.setAttribute('name', 'download_button');
  downloadButton.setAttribute('value', 'Download Results');
  var downloadURL;
  fileSystem.root.getFile('data/rw/last_results_2.7.0.xml', { create: false }, function (fileEntry) {
    downloadURL = fileEntry.toURL();
  });
  downloadButton.addEventListener('click', function (event) {
    console.log("Downloading Results...")
    window.open(downloadURL, 'GLBenchmark 2.7.0 Test Results');
  });
  document.body.appendChild(downloadButton);
  // Create reset button
  var resetButton = document.createElement('input');
  resetButton.setAttribute('type', 'button');
  resetButton.setAttribute('name', 'reset_ext');
  resetButton.setAttribute('value', 'Reload Extension');
  document.body.appendChild(resetButton);
  resetButton.addEventListener('click', function (event) {
    console.log("Reloading extension");
    window.location.reload();
  });


}

// Called after page has loaded
function pageDidLoad() {
  console.log('Page has loaded');
  // Request access to file system
  window.requestFileSystem = window.requestFileSystem || window.webkitRequestFileSystem;
  // Allocate 250MB of persistent storage for this application
  navigator.webkitPersistentStorage.requestQuota(250 * 1024 * 1024, function (bytes) {
	  updateStatus('Allocated ' + bytes + ' bytes of persistant storage');
		window.requestFileSystem(PERSISTENT, bytes, function (fs) {
		  fileSystem = fs;
		  // Only load local files if we are a chrome extension
      if (chrome.extension) getLocalFileList();
      }, errorHandler);
    }, function (e) { alert('Failed to allocate space') }
	);

  if (glBenchModule == null) {
    updateStatus('Loading NaCl Module...');
  } else {
    // If Native Client loaded before page then update status
    updateStatus();
  }
}

// Parse the user input for which GLB test they want to run
function chooseGLBTest() {
  // Check that all files have been loaded and otherwise throw an error
  if (filesLoaded == false) {
    if (chrome.extension) window.alert("Files could not be loaded");
    else window.alert("No data location selected or files could not be loaded");
    return;
  } else {
    // Get selected test from radio buttons
    if (document.getElementById('trex_on').checked) {
      updateStatus("Starting TRex 2_7_0 Onscreen Test...");
      glBenchModule.postMessage("trex_on");
    } else if (document.getElementById('trex_off').checked) {
      updateStatus("Starting TRex 2_7_0 Offscreen Test...");
      glBenchModule.postMessage("trex_off");
    }
    // If there are no more files start the module
    glBenchModule.postMessage("Start");
    // Remove test selection DOM element
    var element = document.getElementById("test_form");
    document.body.removeChild(element);
  }
}

// Set the global status message.  If the element with id 'statusField'
// exists, then set its HTML to the status message as well.
function updateStatus(opt_message) {
  if (opt_message)
    statusText = opt_message;
  var statusField = document.getElementById('statusField');
  if (statusField) {
    statusField.innerHTML = statusText;
  }
}

// Read the list of local files from saved text file
function getLocalFileList() {
  var fileListPath = 'filelist.txt';
  var localFileURL;
  if (chrome.extension) localFileURL = chrome.extension.getURL(fileListPath);
  var txtFile = new XMLHttpRequest();
  txtFile.open("GET", localFileURL, true);
  txtFile.onreadystatechange = function()
  {
    if (txtFile.readyState === 4) {  // document is ready to parse.
      if (txtFile.status === 200) {  // file is found
        allText = txtFile.responseText;
        // Convert all slashes to forward slashes
        lines = txtFile.responseText.replace(/\\/g,"/").split("\n");
        fileList = lines;
        fileCount = fileList.length - 1
        fileIndex = 0;
        handleLocalFile(fileList[0]);
      }
    }
  }
  txtFile.send(null);
}
    
// Function for loading local data files without user input
function handleLocalFile(filename) {
  updateStatus("Loading Asset Files...");
  if (fileIndex == fileCount - 1) {
    updateStatus("Test Assets Loaded");
    filesLoaded = true;
  }
  // Remove unexpected newline characters (i.e. Windows)
  var filePath = filename.replace(/\r?\n|\r/g,"");
  console.log("Loading local file: " + filePath);
  var localFileURL = chrome.extension.getURL(filePath);
  // Create file only if it doesn't already exist
  fileSystem.root.getFile(filePath, { create: false },
    function () {
      // File already exists so do nothing
      console.log("File already exists");
      // Increment index and proceed onto next file
      fileIndex = fileIndex + 1
      if (fileIndex < fileCount) {
        handleLocalFile(fileList[fileIndex]);
      } else {
        // All files have been loaded
        updateStatus("Test Assets Loaded");
        filesLoaded = true;
      }
    }, function () {
      // Download and save the file to filePath
      downloadFile(localFileURL, function (blob) {
        console.log("File download successful");
        saveFile(blob, filePath);
      });
    });
}

// Handle user file selection
function handleFileSelect(evt) {
  console.log("Handling file selection");
  files = evt.target.files;
  fileCount = files.length;
  fileIndex = 0;
  handleFile(files[0]);
}

function handleFile(file) {
  updateStatus("Loading Asset Files...");
  var curFileURL = window.webkitURL.createObjectURL(file);
  console.log(file);
  var fileMessage = file.name + '#' + curFileURL;
  console.log(fileMessage);
  // Check if file exists already
  // Create file only if it doesn't already exist
  fileSystem.root.getFile(file.webkitRelativePath, { create: false },
    function () {
      // File already exists so do nothing
      console.log("File already exists");
      // Increment index and proceed onto next file
      fileIndex = fileIndex + 1
      if (fileIndex < fileCount) {
        handleFile(files[fileIndex]);
      } else {
        updateStatus("Test Assets Loaded");
        filesLoaded = true;
      }
    }, function () {
      // Download and save file to disk
      downloadFile(curFileURL, function (blob) {
        console.log("File download successful");
        saveFile(blob, file.webkitRelativePath);
      });
    });
}

// Download data from given URL
function downloadFile(url, success) {
	console.log("Downloading url");
	console.log(url);
  // Create HTTP request
	var xhr = new XMLHttpRequest();
	xhr.open('GET', url, true);
	xhr.responseType = "blob";
	xhr.onreadystatechange = function () {
	  if (xhr.readyState == 4) {
      // Call success callback
	    if (success) success(xhr.response);
	  }
	};
  // Close request
	xhr.send(null);
}

// Save specified data at given path
function saveFile(data, path) {
	console.log("Preparing to save file to system");
	// Check that filesystem exists
	if (!fileSystem) return;
	// Create folders corresponding to path
	var folders = path.split('/');
	folders.pop();
	createDir(fileSystem.root, folders);
	// Save file to disk
	fileSystem.root.getFile(path, { create: true, exclusive: false }, function (fileEntry) {
	  fileEntry.createWriter(function (writer) {
	    console.log("Writing file data");
	    console.log(data);
	    writer.write(data);
	  }, errorHandler);
	}, errorHandler);
	// Increment index and proceed onto next file
	fileIndex = fileIndex + 1
	if (fileIndex < fileCount) {
    // If we are using local files then handle them otherwise handle user inputed files
    if (files == null) handleLocalFile(fileList[fileIndex]);
    else handleFile(files[fileIndex]);
	}
}

// Create given directory hierarchy
function createDir(rootDir, folders) {
	rootDir.getDirectory(folders[0], { create: true }, function (dirEntry) {
	  if (folders.length) {
	    createDir(dirEntry, folders.slice(1));
	  }
	}, errorHandler);
}

// Error handler for downloading and saving file to disk
function errorHandler(e) {
	console.log('Error: ' + e.name + " " + e.message);
}

// Function for selecting next test - not used
function nextTest () {
  console.log("Ending test");
  glBenchModule.postMessage("NextTest");
}

// Add the required listeners and do any modifications to DOM content after it is loaded
document.addEventListener('DOMContentLoaded', function () {
  // Add listeners to NaCl module
  var listener = document.getElementById('listener');
  listener.addEventListener('load', moduleDidLoad, true);
  listener.addEventListener('message', handleMessage, true);
  document.getElementById("test_select_submit").addEventListener('click', chooseGLBTest, true);
  // Depending on whether application is an extension, remove file_input element
  if (chrome.extension) {
    var element = document.getElementById("file_input");
    document.body.removeChild(element);
  } else {
    document.getElementById('file_input').addEventListener('change', handleFileSelect, false);
  }
});

document.addEventListener('onload', pageDidLoad());