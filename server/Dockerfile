FROM python:3.12-slim

ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    HOST=0.0.0.0 \
    PORT=1357 \
    DATABASE_FILE=/data/defensive.db

WORKDIR /app

COPY . .

RUN mkdir -p /data

EXPOSE 1357

HEALTHCHECK --interval=30s --timeout=5s --start-period=10s --retries=3 \
    CMD python -c "import os, socket; s=socket.create_connection(('127.0.0.1', int(os.getenv('PORT', '1357'))), 3); s.close()"

CMD ["python", "server.py"]
