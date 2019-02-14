node('builder'){
    docker.image('riddleandcode/wallet-builder').inside('--privileged') {
        checkout scm
        version = readFile "${env.WORKSPACE}/version"
        stage('Generating build') {
            sh 'mkdir -p build && cd build && cmake ../ '
        }
        stage('Coding Guideline') {
                sh 'astyle "lib/*.c" "lib/*.h" "lib/hal/*.c" "lib/hal/*.h" "lib/basic/*.c" "lib/basic/*.h"  "lib/host/*.c" "lib/host/*.h" "test/*.c" "test/*.h" --style=google -s2'
                //sh 'echo \'if [ $(find . -iname "*.orig" | wc -l) -eq 0 ]; then echo "According to guideline."; else echo "Not according to guideline" && exit 1; fi\' > guide && sh guide'
        }
        dir('build')
        {
            stage('Build') {
                sh 'make'
            }
            /*
            stage('Testing') {
                sh 'make check'
                sh 'xsltproc /opt/ctest/ctest2junix.xsl tests/Testing/$(head -1 tests/Testing/TAG)/Test.xml > CTestResults.xml '
                junit 'CTestResults.xml'
                cobertura coberturaReportFile: 'coverage.xml'
            }
            */
            stage("Upload to Artifactory"){
                rtUpload (
                    serverId: "ArtifactoryOSS", // Obtain an Artifactory server instance, defined in Jenkins --> Manage:
                    spec: """{
                        "files": [
                            {
                                "pattern": "lib/libcryptoauth.a",
                                "target": "generic-local/release-candidate/cryptoauthlib/${version}/libcryptoauth.a"
                            },
                            {
                                "pattern": "lib/libcryptoauthdyn.so",
                                "target": "generic-local/release-candidate/cryptoauthlib/${version}/libcryptoauth.so"
                            }
                        ]
                    }"""
                )
                rtPublishBuildInfo (
                    serverId: "ArtifactoryOSS"
                )
           }
        }
    }
}
