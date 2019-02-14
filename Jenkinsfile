node('builder'){
    docker.image('riddleandcode/wallet-builder').inside('--privileged') {
        checkout scm
        version = readFile "${env.WORKSPACE}/VERSION"
        versionEscaped = sh "echo version | tr -d '\n'"
        stage('Generating build') {
            sh 'mkdir -p build && cd build && cmake ../ -DTARGET_GROUP=all -DSTATIC_ANALYSIS=1  '
        }
        stage('Coding Guideline') {
                sh 'astyle "src/*.c" "include/*.h" "tests/*.c" "tests/*.h" --style=google -s2'
                sh 'echo \'if [ $(find . -iname "*.orig" | wc -l) -eq 0 ]; then echo "According to guideline."; else echo "Not according to guideline" && exit 1; fi\' > guide && sh guide'
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
                                "pattern": "build/lib/libcryptoauth.a",
                                "target": "generic-local/release-candidate/cryptoauthlib/$versionEscaped/libcryptoauth.a"
                            },
                            {
                                "pattern": "build/lib/libcryptoauth.so",
                                "target": "generic-local/release-candidate/cryptoauthlib/$versionEscaped/libcryptoauth.so"
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
