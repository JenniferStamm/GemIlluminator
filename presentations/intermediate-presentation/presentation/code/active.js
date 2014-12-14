Connections {
	target: Qt.application

	onStateChanged: {
		switch (Qt.application.state) {
		case Qt.ApplicationSuspended:
			scene.active = false
			console.log("Suspended")
			break
		case Qt.ApplicationHidden:
			scene.active = false
			console.log("Hidden")
			break
		case Qt.ApplicationActive:
			if(Qt.platform.os == "android") {
				root.showFullScreen()

				var types = QmlSensors.sensorTypes();

				if (types.indexOf("QRotationSensor") !== -1 && Qt.platform.os == "android") {
					sensorInputs.rotationSensorId.active = true
				}
				else if (types.indexOf("QTiltSensor") !== -1 && Qt.platform.os == "android") {
					sensorInputs.tiltSensorId.active = true
				}	
				else {
						mouseInput.enabled = true
					}
				}
			scene.active = true
			console.log("Active")
			break
		case Qt.ApplicationInactive:
			scene.active = false

			if(Qt.platform.os == "android") {
				root.hide()
			}

			console.log("Inactive")
			break
		}
   }
}