Connections {
	target: Qt.application

	onStateChanged: {
		switch (Qt.application.state) {
		case Qt.ApplicationActive:
			if(Qt.platform.os == "android") {
				root.showFullScreen()
				var types = QmlSensors.sensorTypes();

				if (types.indexOf("QRotationSensor") !== -1) {
					sensorInputs.rotationSensorId.active = true
				} else if (types.indexOf("QTiltSensor") !== -1) {
					sensorInputs.tiltSensorId.active = true
				}  
			}
			else {
				mouseInput.enabled = true
			}

			if(scene !== null) 
				scene.active = true
			console.log("Active")
			break
		case Qt.ApplicationInactive:
			if(scene !== null) 
				scene.active = false

			if(Qt.platform.os === "android") 
				root.hide()

			console.log("Inactive")
			break
		case Qt.ApplicationSuspended:
			if(scene !== null) 
				scene.active = false
			console.log("Suspended")
			break
		case Qt.ApplicationHidden:
			if(scene !== null) 
				scene.active = false
			console.log("Hidden")
			break
		}
	}
}