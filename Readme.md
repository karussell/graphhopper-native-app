# Native GraphHopper

Calculate offline routes and distances with GraphHopper on the Android
device using a native image of GraphHopper instead of the jar dependency.

It already works :) ! And performance is "okay". E.g. 300km route within 
2 seconds on a OnePlus 5T device.

There are the following advantages:

 * no strange per-process virtual memory limit
 * GraphHopper does not need to limit itself to Java 7
 * this process works with Gluon and JavaFX
 * same process could work for iOS
 * public transit could work too
 * data import of raw OSM files (especially for small areas) could work too (i.e. on-demand areas and routes could be possible)

The main disadvante is the complexity of the process: you not only need the
jar but the Android NDK & SDK and GraalVM and a maven plugin to create a
native library that apps can integrate only via Android NDK.
See also TODOs.

## Get Started

### Build the Graph Files

 * Get the repository https://github.com/karussell/graphhopper-native-image/
 * mvn clean install
 * download a pbf file from OSM e.g. `wget http://download.geofabrik.de/europe/germany/berlin-latest.osm.pbf`
 * `ln -s berlin-latest.osm.pbf osm.pbf`
 * `java -jar target/graphhoppernative-1.0-SNAPSHOT-jar-with-dependencies.jar`
 * `adb push graph-cache/ /data/local/tmp/`

### Build the Library

Follow instructions in https://github.com/karussell/graphhopper-native-image/pull/2

Copy the created `libgraphhoppernative.so` into `distribution/ghlib/arm64-v8a/`

## TODOs

 * Communication with library is unclear. Currently we can only send strings
 * What other targets besides arm64-v8a can we support?
 * Can we avoid app crashes when doing multiple route calculations (how is memory management done?)
 * There are app crashes when doing longer routes (memory problems)