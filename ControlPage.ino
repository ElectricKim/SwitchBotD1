const char* controlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="ko">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>IoT Switch Bot</title>
  <style>
    body {
      height: 100vh;
      margin: 0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      text-align: center;
    }
    .switch {
      display: inline-block;
      position: relative;
      width: 80px;
      height: 80px;
      background: #ccc;
      border-radius: 50%;
      cursor: pointer;
      text-align: center;
      vertical-align: middle;
      margin-bottom: 20px;
    }
    .switch svg {
      width: 50%;
      height: auto;
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
      transition: fill 0.3s ease;
      pointer-events: none;
    }
    .switch.active {
      background: #8cd85a;
    }
    .switch.active svg {
      fill: #8cd85a;
    }
  </style>
</head>
<body>
  <label class="switch" id="switch">
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 118.358 126.445">
      <g fill="#fff">
        <path d="M118.33 67.29c0-24.108-14.409-44.778-35.071-54.02v21.667c9.901 7.38 16.368 19.054 16.4 32.353-.066 22.344-18.132 40.453-40.484 40.485-22.36-.032-40.45-18.14-40.484-40.485 0-13.298 6.515-24.973 16.392-32.353V13.254C14.421 22.495-.004 43.174-.004 67.29c0 32.689 26.484 59.155 59.179 59.155 32.663 0 59.155-26.467 59.179-59.155h-.03"/>
          <path d="M71.25 79.53c0 .55-.45 1-1 1H48.099c-.55 0-1-.45-1-1V1c0-.55.45-1 1-1h22.15c.55 0 1 .45 1 1v78.53"/>
      </g>
    </svg>
  </label>
  <script>
    const arduinoUrl = window.location.origin;
    const switchElement = document.querySelector('.switch');

    const updateSwitchState = async () => {
      try {
        const response = await fetch(`${arduinoUrl}/switch`);
        const data = await response.json();
        switchElement.classList.toggle('active', data.isSwitchOn);
      } catch (error) {
        console.error('스위치 상태를 가져오는 데 실패했습니다:', error);
      }
    };

    updateSwitchState();

    switchElement.addEventListener('click', async () => {
      try {
        const isActive = switchElement.classList.contains('active');
        const endpoint = isActive ? `${arduinoUrl}/switch-off` : `${arduinoUrl}/switch-on`;
        const response = await fetch(endpoint);
        if (!response.ok) {
          throw new Error('네트워크 응답이 올바르지 않습니다');
        }
        const data = await response.json();
        switchElement.classList.toggle('active', data.isSwitchOn);
      } catch (error) {
        console.error('아두이노에 요청을 보내는 데 실패했습니다:', error);
      }
    });
  </script>
</body>
</html>
)rawliteral";