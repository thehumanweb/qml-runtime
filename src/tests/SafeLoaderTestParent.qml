import QtQuick 2.0
import testing 1.0

Item {
    id: container

    SafeLoaderTestHelper {
        id: helper
        loader: loader
        objectName: "parentHelper"
        onNotifyQml: {
            if (helper.properties.introspect) {
                var properties = {
                    children: loader.children,
                    resources: loader.resources,
                    global_object: __global_object,
                    introspect_done: true
                }
                helper.properties = properties
                return
            }
            var source = helper.properties.source
            loader.source = source
        }
    }

    SafeLoader {
        id: loader
    }

    Item {
        id: __global_object
    }

    Component.onCompleted: {
        helper.properties = {ready: true}
    }
}
