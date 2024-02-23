import React from 'react';
import { render } from '@testing-library/react';
import Lines from './Lines';

jest.mock("../../components/header/Header", () => () => <div data-testid="header">Mocked Header</div>);
jest.mock("../../components/footer/Footer", () => () => <div data-testid="footer">Mocked Footer</div>);
jest.mock("../../components/listLine/ListLine", () => () => <div data-testid="listLine">Mocked ListLine</div>);

describe('Lines', () => {
  it('renders header, list line, and footer', () => {
    const { getByTestId } = render(<Lines />);
    
    expect(getByTestId('header')).toBeInTheDocument();
    expect(getByTestId('listLine')).toBeInTheDocument();
    expect(getByTestId('footer')).toBeInTheDocument();
  });
});
