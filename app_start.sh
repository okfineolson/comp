# commands to start apps
#!/bin/bash
cd /home/ubuntu/aws-code-deploy-demo/docker
docker-compose up -d --force-recreate --no-deps --build
