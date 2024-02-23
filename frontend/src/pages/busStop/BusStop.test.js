import React from 'react';
import { render } from '@testing-library/react';
import BusStop from './BusStop';

jest.mock("../../components/busStopList/BusStopList", () => () => <div data-testid="busStopList">Mocked BusStopList</div>);
jest.mock("../../components/footerBusStop/FooterBusStop", () => () => <div data-testid="footerBusStop">Mocked FooterBusStop</div>);
jest.mock("../../components/headerBusStop/HeaderBusStop", () => () => <div data-testid="headerBusStop">Mocked HeaderBusStop</div>);

describe('BusStop', () => {
  it('renders header, bus stop list, and footer', () => {
    const { getByTestId } = render(<BusStop />);
    
    expect(getByTestId('headerBusStop')).toBeInTheDocument();
    expect(getByTestId('busStopList')).toBeInTheDocument();
    expect(getByTestId('footerBusStop')).toBeInTheDocument();
  });
});
