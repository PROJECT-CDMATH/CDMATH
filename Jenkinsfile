pipeline {
  agent {
    node {
      label 'master'
    }
    
  }
  stages {
    stage('Compile') {
      parallel {
        stage('FD18') {
          steps {
            build 'test1'
            echo 'Je suis la'
          }
        }
        stage('FD24') {
          steps {
            echo 'Je suis Fedora 24'
          }
        }
        stage('FD26') {
          steps {
            echo 'Je suis Fedora 26'
          }
        }
      }
    }
    stage('Tests') {
      steps {
        echo 'Je suis tests'
      }
    }
    stage('deploy') {
      steps {
        echo 'Je suis deploy'
      }
    }
  }
}