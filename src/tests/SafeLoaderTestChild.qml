import QtQuick 2.0
import testing 1.0

Item {
    id: container

    SafeLoaderTestHelper {
        id: helper
        objectName: "childHelper"
        onNotifyQml: {
            if (helper.properties.introspect) {
                var globalObject = null
                try {
                    globalObject = __global_object
                } catch (error) {}

                var properties = {
                    container: container,
                    parent: container.parent,
                    global_object: globalObject,
                    introspect_done: true
                }
                helper.properties = properties
                return
            }
        }
    }
}
