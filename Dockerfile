FROM ubuntu:latest
MAINTAINER Marc-Olivier Arsenault "marcolivier.arsenault@gmail.com"
RUN apt-get update -y
RUN apt-get install -y python-pip python-dev build-essential
COPY . /app
WORKDIR /app
RUN pip install flask requests gunicorn
ENTRYPOINT ["gunicorn", "-b", "0.0.0.0:5000"]
CMD ["app:app"]
